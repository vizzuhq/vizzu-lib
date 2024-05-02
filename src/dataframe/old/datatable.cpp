#include "datatable.h"

#include <cmath>
#include <numeric>
#include <utility>

#include "base/conv/auto_json.h"
#include "base/text/funcstring.h"
#include "chart/options/options.h"
#include "dataframe/impl/aggregators.h"
#include "dataframe/interface.h"

namespace Vizzu::Data
{

void DataTable::addColumn(std::string_view name,
    std::string_view unit,
    const std::span<const double> &values)
{
	df.add_measure(values,
	    name.data(),
	    dataframe::adding_type::create_or_override,
	    {{std::pair{"unit", unit.data()}}});
}

void DataTable::addColumn(std::string_view name,
    const std::span<const char *const> &categories,
    const std::span<const std::uint32_t> &values)
{
	df.add_dimension(categories,
	    values,
	    name.data(),
	    dataframe::adding_type::create_or_override,
	    {});
}

void DataTable::pushRow(const std::span<const char *> &cells)
{
	df.add_record(cells);
}

std::string DataTable::getInfos() const { return df.as_string(); }

bool SeriesIndex::isDimension() const { return !aggr; }

const std::string_view &SeriesIndex::getColIndex() const
{
	return sid;
}

bool SeriesIndex::operator==(const SeriesIndex &rhs) const
{
	return sid == rhs.sid && aggr == rhs.aggr;
}

bool SeriesIndex::operator<(const SeriesIndex &rhs) const
{
	return sid < rhs.sid || (sid == rhs.sid && aggr < rhs.aggr);
}

bool operator<(const std::string_view &dim, const SeriesIndex &rhs)
{
	return dim < rhs.getColIndex() || !rhs.isDimension();
}

bool operator<(const SeriesIndex &lhs, const std::string_view &dim)
{
	return lhs.getColIndex() < dim && lhs.isDimension();
}

bool SliceIndex::operator<(SliceIndex const &rhs) const
{
	return column < rhs.column
	    || (column == rhs.column && value < rhs.value);
}

bool MultiIndex::isEmpty() const { return !rid; }

bool MarkerId::operator==(const MarkerId &id) const
{
	return itemId == id.itemId && seriesId == id.seriesId;
}

bool DataCube::iterator_t::operator!=(const iterator_t &oth) const
{
	return parent != oth.parent;
}

void DataCube::iterator_t::operator++() { parent->incr(*this); }

const MultiIndex &DataCube::iterator_t::operator*() const
{
	return index;
}

SeriesIndex::SeriesIndex(std::string const &str,
    const DataTable &table) :
    orig_name(str)
{
	constinit static auto names =
	    Refl::get_names<dataframe::aggregator_type>();
	if (const Text::FuncString func(str, false);
	    !func.isEmpty()
	    && std::find(names.begin(), names.end(), func.getName())
	           != names.end()) {
		aggr = Refl::get_enum<dataframe::aggregator_type>(
		    func.getName());
		if (!func.getParams().empty())
			sid = table.getDf()
			          .get_series_meta(func.getParams().at(0))
			          .name;
	}
	else {
		auto &&[s, type] = table.getDf().get_series_meta(str);
		sid = s;
		if (type == DataTable::Type::measure)
			aggr = dataframe::aggregator_type::sum;
	}
}

DataCube::iterator_t DataCube::begin() const
{
	if (df->get_record_count() == 0) return {};
	iterator_t res{this,
	    {},
	    {{}, std::vector<std::size_t>(dim_reindex.size())}};
	check(res);
	return res;
}

DataCube::iterator_t DataCube::end() { return {}; }

void DataCube::check(iterator_t &it) const
{
	if (it.index.rid) {
		++it.rid;
		it.index.rid.reset();
	}

	for (std::size_t ix{}; ix < dim_reindex.size(); ++ix) {
		auto &&[dim, cats, size] = dim_reindex[ix];
		if (auto &&old_ix = it.index.old[ix];
		    (old_ix < cats.size() ? cats[old_ix].data() : nullptr)
		    != std::get<std::string_view>(df->get_data(it.rid, dim))
		           .data())
			return;
	}
	it.index.rid.emplace(it.rid);
}

void DataCube::incr(iterator_t &it) const
{
	for (std::size_t ix{dim_reindex.size()}; ix-- > 0;)
		if (++it.index.old[ix] >= dim_reindex[ix].size)
			it.index.old[ix] = 0;
		else {
			check(it);
			return;
		}
	it.parent = nullptr;
}

DataCube::DataCube(const DataTable &table,
    const Gen::Options &options)
{
	auto &&channels = options.getChannels();
	auto &&dimensions = channels.getDimensions();
	auto &&measures = channels.getMeasures();
	auto empty = dimensions.empty() && measures.empty();

	df = {empty ? dataframe::dataframe::create_new()
	            : table.getDf().copy(false)};

	if (empty) {
		df->finalize();
		return;
	}

	dim_reindex.resize(dimensions.size());
	df->remove_records(options.dataFilter.getFunction());

	auto removed = df->copy(false);

	for (const auto &dim : dimensions)
		df->aggregate_by(dim.getColIndex());

	for (const auto &meas : measures) {
		auto &&sid = meas.getColIndex();
		auto &&aggr = meas.getAggr();

		measure_names.try_emplace(std::pair{sid, aggr},
		    df->set_aggregate(sid, aggr));
	}

	for (const auto &dim : dimensions) {
		df->set_sort(dim.getColIndex(),
		    dataframe::sort_type::by_categories,
		    dataframe::na_position::last);
	}

	df->finalize();

	for (auto it = dim_reindex.begin();
	     const auto &dim : dimensions) {
		auto &&dimName = dim.getColIndex();
		auto &&cats = df->get_categories(dimName);
		*it++ = {*std::lower_bound(df->get_dimensions().begin(),
		             df->get_dimensions().end(),
		             dimName),
		    cats,
		    cats.size() + df->has_na(dimName)};
	}

	auto stackInhibitingShape =
	    options.geometry == Gen::ShapeType::area;
	auto horizontal = options.isHorizontal();

	using ChannelId = Gen::ChannelId;
	for (auto &&[channelId, inhibitStack] :
	    std::initializer_list<std::pair<ChannelId, bool>>{
	        {ChannelId::size, false},
	        {ChannelId::x, !horizontal && stackInhibitingShape},
	        {ChannelId::y, horizontal && stackInhibitingShape}}) {
		const auto &channel = channels.at(channelId);
		auto &&meas = channel.measureId;
		if (!meas) continue;
		const auto *subChannel = options.subAxisOf(channelId);
		if (!subChannel) continue;
		auto sumBy = subChannel->dimensionIds;
		if (auto &&common = sumBy.split_by(channel.dimensionIds);
		    inhibitStack)
			std::swap(sumBy, common);
		if (sumBy.empty()) continue;

		auto &sub_df =
		    *cacheImpl.try_emplace(channelId, removed->copy(false))
		         .first->second;

		for (auto &&dim : dimensions)
			if (!sumBy.contains(dim))
				sub_df.aggregate_by(dim.getColIndex());

		[[maybe_unused]] auto &&new_name =
		    sub_df.set_aggregate(meas->getColIndex(),
		        meas->getAggr());

		sub_df.finalize();
	}
}

std::pair<size_t, size_t> DataCube::combinedSizeOf(
    const SeriesList &colIndices) const
{
	std::size_t maxBySL{1};
	std::size_t maxByOthers{1};

	std::set<std::string_view> sls;
	for (auto &&s : colIndices) sls.insert(s.getColIndex());
	for (auto &&[n, cats, s] : dim_reindex)
		if (sls.contains(n))
			maxBySL *= s;
		else
			maxByOthers *= s;

	return {maxByOthers, maxBySL};
}

bool DataCube::empty() const
{
	return df->get_measures().empty() && df->get_dimensions().empty();
}

const std::string &DataCube::getName(
    const SeriesIndex &seriesId) const
{
	return measure_names.at(
	    {seriesId.getColIndex(), seriesId.getAggr()});
}

std::string_view DataCube::getUnit(
    std::string_view const &colIx) const
{
	return df->get_series_info(colIx, "unit");
}

MarkerId DataCube::getId(const SeriesList &sl,
    const MultiIndex &mi) const
{
	MarkerId res{SubSliceIndex(sl.size())};
	std::vector<std::pair<std::size_t, std::size_t>> v(sl.size());

	for (std::size_t ix{}; ix < dim_reindex.size(); ++ix) {
		auto &&[name, cats, size] = dim_reindex[ix];
		auto &&oldIx = mi.old[ix];

		if (auto &&i = sl.find(name); i != SeriesList::npos) {
			v[i] = {oldIx, size};
			res.itemSliceIndex[i] = {name,
			    oldIx < cats.size() ? cats[oldIx]
			                        : std::string_view{}};
		}
		else
			res.seriesId = res.seriesId * size + oldIx;
	}

	for (const auto &[cix, size] : v)
		res.itemId = res.itemId * size + cix;

	return res;
}

std::shared_ptr<CellInfo> DataCube::cellInfo(
    const MultiIndex &index) const
{
	auto my_res = std::make_shared<CellInfo>(
	    std::vector<std::pair<std::string, std::string>>(
	        dim_reindex.size()),
	    std::vector<std::pair<std::string, double>>(
	        df->get_measures().size()));

	for (std::size_t ix{}; ix < dim_reindex.size(); ++ix) {
		auto &&[name, cats, size] = dim_reindex[ix];
		auto cix = index.old[ix];
		my_res->categories[ix] = {std::string{name},
		    cix < cats.size() ? cats[cix] : std::string{}};
	}

	for (std::size_t ix{}; auto &&meas : df->get_measures())
		my_res->values[ix++] = {meas,
		    index.rid
		        ? std::get<double>(df->get_data(*index.rid, meas))
		        : 0.0};

	return my_res;
}

double DataCube::valueAt(const MultiIndex &multiIndex,
    const SeriesIndex &seriesId) const
{
	if (multiIndex.rid)
		return std::get<double>(
		    df->get_data(*multiIndex.rid, getName(seriesId)));

	return {};
}

double DataCube::aggregateAt(const MultiIndex &multiIndex,
    const Gen::ChannelId &channelId,
    const SeriesIndex &seriesId) const
{
	auto it = cacheImpl.find(channelId);
	if (it == cacheImpl.end()) return valueAt(multiIndex, seriesId);

	auto &sub_df = *it->second;
	const auto &name = getName(seriesId);

	if (multiIndex.rid) {
		auto &&rrid = df->get_record_id_by_dims(*multiIndex.rid,
		    sub_df.get_dimensions());
		return std::get<double>(sub_df.get_data(rrid, name));
	}

	// hack for sunburst.
	std::map<std::string_view, std::size_t> index;

	for (std::size_t ix{}; ix < dim_reindex.size(); ++ix)
		index.emplace(dim_reindex[ix].name, multiIndex.old[ix]);

	std::string res;
	for (auto iit = index.begin();
	     auto &&dim : sub_df.get_dimensions()) {
		if (iit->first != dim) ++iit;

		auto &&cats = df->get_categories(dim);
		res += dim;
		res += ':';
		res += iit->second == cats.size() ? "__null__"
		                                  : cats[iit->second];
		res += ';';
	}
	auto nanres = std::get<double>(sub_df.get_data(res, name));
	return std::isnan(nanres) ? double{} : nanres;
}

} // namespace Vizzu::Data
