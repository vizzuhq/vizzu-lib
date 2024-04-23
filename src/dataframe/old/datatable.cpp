#include "datatable.h"

#include <base/conv/auto_json.h>
#include <base/text/funcstring.h>
#include <cmath>
#include <data/datacube/datacube.h>
#include <dataframe/impl/aggregators.h>
#include <dataframe/interface.h>
#include <numeric>
#include <utility>

#include "data/datacube/datacubeoptions.h"
#include "data/datacube/datafilter.h"
#include "data/datacube/seriesindex.h"
#include "data/table/datatable.h"

namespace Vizzu::Data
{

void data_table::addColumn(std::string_view name,
    std::string_view unit,
    const std::span<const double> &values)
{
	df.add_measure(values,
	    name.data(),
	    dataframe::adding_type::create_or_override,
	    {{std::pair{"unit", unit.data()}}});
}

void data_table::addColumn(std::string_view name,
    const std::span<const char *const> &categories,
    const std::span<const std::uint32_t> &values)
{
	df.add_dimension(categories,
	    values,
	    name.data(),
	    dataframe::adding_type::create_or_override,
	    {});
}

void data_table::pushRow(const std::span<const char *> &cells)
{
	df.add_record(cells);
}

std::string data_table::getInfos() const { return df.as_string(); }

bool series_index_t::isDimension() const { return !aggr; }

const std::string_view &series_index_t::getColIndex() const
{
	return sid;
}

bool series_index_t::operator==(const series_index_t &rhs) const
{
	return sid == rhs.sid && aggr == rhs.aggr;
}

bool series_index_t::operator<(const series_index_t &rhs) const
{
	return sid < rhs.sid || (sid == rhs.sid && aggr < rhs.aggr);
}

bool slice_index_t::operator<(slice_index_t const &rhs) const
{
	return column < rhs.column
	    || (column == rhs.column && value < rhs.value);
}

bool data_cube_t::multi_index_t::isEmpty() const { return !rid; }

bool data_cube_t::Id::operator==(const Id &id) const
{
	return itemSliceIndex == id.itemSliceIndex
	    && seriesId == id.seriesId;
}

bool data_cube_t::data_t::iterator_t::operator!=(
    const iterator_t &oth) const
{
	return parent != oth.parent;
}

void data_cube_t::data_t::iterator_t::operator++()
{
	parent->incr(*this);
}

const data_cube_t::multi_index_t &
data_cube_t::data_t::iterator_t::operator*() const
{
	return index;
}

series_index_t::series_index_t(std::string const &str,
    const data_table &table) :
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

data_cube_t::data_t::iterator_t data_cube_t::data_t::begin() const
{
	if (df->get_record_count() == 0) return {};
	iterator_t res{this,
	    {},
	    {{}, std::vector<std::size_t>(sizes.size())}};
	check(res);
	return res;
}

data_cube_t::data_t::iterator_t data_cube_t::data_t::end()
{
	return {};
}

void data_cube_t::data_t::check(iterator_t &it) const
{
	if (it.index.rid) {
		++it.rid;
		it.index.rid.reset();
	}

	for (std::size_t ix{}; ix < dim_reindex.size(); ++ix) {
		auto &&dim = dim_reindex[ix];
		auto &&cats = df->get_categories(dim);
		auto val =
		    std::get<std::string_view>(df->get_data(it.rid, dim));
		if (cats.size() == it.index.old[ix]) {
			if (val.data() != nullptr) { return; }
		}
		else if (cats[it.index.old[ix]] != val) {
			return;
		}
	}
	it.index.rid.emplace(it.rid);
}

void data_cube_t::data_t::incr(iterator_t &it) const
{
	for (std::size_t ix{sizes.size()}; ix-- > 0;)
		if (++it.index.old[ix] >= sizes[ix])
			it.index.old[ix] = 0;
		else {
			check(it);
			return;
		}
	it.parent = nullptr;
}

template <>
data_cube_t::data_cube_t(const data_table &table,
    const DataCubeOptions &options,
    const Filter &filter) :
    table(&table),
    data{options.getDimensions().empty()
                 && options.getMeasures().empty()
             ? dataframe::dataframe::create_new()
             : table.getDf().copy(false),
        options}
{
	data.df->remove_records(filter.getFunction());

	removed = data.df->copy(false);

	for (const auto &dim : options.getDimensions())
		data.df->aggregate_by(dim.getColIndex());

	for (const auto &meas : options.getMeasures()) {
		auto &&sid = meas.getColIndex();
		auto &&aggr = meas.getAggr();

		measure_names.try_emplace(std::pair{sid, aggr},
		    data.df->set_aggregate(sid, aggr));
	}

	for (const auto &dim : options.getDimensions()) {
		data.df->set_sort(dim.getColIndex(),
		    dataframe::sort_type::by_categories,
		    dataframe::na_position::last);
	}

	data.df->finalize();

	auto it = data.dim_reindex.begin();
	for (auto sizIt = data.sizes.begin();
	     const auto &dim : options.getDimensions()) {
		auto &&dimName = dim.getColIndex();
		*it++ = *std::lower_bound(data.df->get_dimensions().begin(),
		    data.df->get_dimensions().end(),
		    dimName);
		*sizIt++ = data.df->get_categories(dimName).size()
		         + data.df->has_na(dimName);
	}
}

size_t data_cube_t::combinedSizeOf(
    const series_index_list_t &colIndices) const
{
	std::size_t my_res{1};
	for (const auto &si : colIndices) {
		auto sid = si.getColIndex();
		my_res *= data.df->get_categories(sid).size()
		        + data.df->has_na(sid);
	}
	return my_res;
}

bool data_cube_t::empty() const
{
	return data.df->get_measures().empty()
	    && data.df->get_dimensions().empty();
}

std::string data_cube_t::getValue(const slice_index_t &index)
{
	return std::string{index.value};
}

const std::string &data_cube_t::getName(
    const series_index_t &seriesId) const
{
	return measure_names.at(
	    {seriesId.getColIndex(), seriesId.getAggr()});
}

data_cube_t::Id data_cube_t::getId(const series_index_list_t &sl,
    const multi_index_t &mi) const
{
	std::map<std::string_view, std::size_t> reindex;
	struct DimProp
	{
		std::string_view value;
		std::size_t index;
		std::size_t size;
	};
	std::vector<std::pair<std::string_view, DimProp>> v;
	for (std::size_t ix{}; const auto &s : sl)
		reindex[v.emplace_back(s.getColIndex(), DimProp{}).first] =
		    ix++;

	std::size_t seriesId{};
	for (std::size_t ix{}; ix < data.dim_reindex.size(); ++ix) {
		auto &&name = data.dim_reindex[ix];
		auto &&size = data.sizes[ix];
		if (auto it = reindex.find(name); it != reindex.end()) {
			auto &[cat, cix, csize] = v[it->second].second;
			cix = mi.old[ix];
			csize = size;
			if (auto &&cats = data.df->get_categories(name);
			    cix < cats.size())
				cat = cats[cix];
		}
		else {
			seriesId *= size;
			seriesId += mi.old[ix];
		}
	}

	Id::SubSliceIndex ssi;
	std::size_t itemId{};
	for (std::size_t i{}; i < v.size(); ++i) {
		auto &[cat, cix, size] = v[i].second;

		ssi.emplace_back(v[i].first, cat);
		itemId *= size;
		itemId += cix;
	}

	return {mi, ssi, seriesId, itemId};
}

data_cube_t::CellInfo data_cube_t::cellInfo(
    const multi_index_t &index) const
{
	CellInfo my_res;

	for (std::size_t ix{}; ix < data.dim_reindex.size(); ++ix) {
		auto &&name = data.dim_reindex[ix];
		auto cats = data.df->get_categories(name);
		auto cix = index.old[ix];
		my_res.categories.emplace_back(name,
		    cix < cats.size() ? cats[cix] : std::string_view{});
	}

	for (auto &&meas : data.df->get_measures())
		my_res.values.emplace_back(meas,
		    index.rid ? std::get<double>(
		        data.df->get_data(*index.rid, meas))
		              : 0.0);

	return my_res;
}

double data_cube_t::valueAt(const multi_index_t &multiIndex,
    const series_index_t &seriesId) const
{
	if (multiIndex.rid) {
		const auto &name = getName(seriesId);
		return std::get<double>(
		    data.df->get_data(*multiIndex.rid, name));
	}

	return {};
}

double data_cube_t::aggregateAt(const multi_index_t &multiIndex,
    const series_index_list_t &sumCols,
    const series_index_t &seriesId) const
{
	if (sumCols.empty()) return valueAt(multiIndex, seriesId);

	std::string id{};
	for (const auto &s : sumCols) {
		id += s.getColIndex();
		id += ';';
	}

	const auto &name = getName(seriesId);

	id += name;

	auto it = cacheImpl->find(id);
	if (it == cacheImpl->end()) {
		it = cacheImpl->emplace(id, removed->copy(false)).first;

		auto &cp = it->second;

		auto keep_this = std::set<std::string_view>(
		    data.df->get_dimensions().begin(),
		    data.df->get_dimensions().end());
		for (const auto &dim : sumCols)
			keep_this.erase(dim.getColIndex());

		for (const auto &dim : keep_this) cp->aggregate_by(dim);

		[[maybe_unused]] auto &&new_name =
		    cp->set_aggregate(seriesId.getColIndex(),
		        seriesId.getAggr());

		for (auto &&dim : keep_this)
			cp->set_sort(dim,
			    dataframe::sort_type::by_categories,
			    dataframe::na_position::last);

		cp->finalize();
	}

	auto &sub_df = it->second;

	if (multiIndex.rid) {
		auto &&rrid = data.df->get_record_id_by_dims(*multiIndex.rid,
		    sub_df->get_dimensions());
		return std::get<double>(sub_df->get_data(rrid, name));
	}

	// hack for sunburst.
	std::map<std::string_view, std::size_t> index;

	for (std::size_t ix{}; ix < data.dim_reindex.size(); ++ix)
		index.emplace(data.dim_reindex[ix], multiIndex.old[ix]);

	std::string res;
	for (auto iit = index.begin();
	     auto &&dim : sub_df->get_dimensions()) {
		if (iit->first != dim) ++iit;

		auto &&cats = data.df->get_categories(dim);
		res += dim;
		res += ':';
		res += iit->second == cats.size() ? "__null__"
		                                  : cats[iit->second];
		res += ';';
	}
	auto nanres = std::get<double>(sub_df->get_data(res, name));
	return std::isnan(nanres) ? double{} : nanres;
}

} // namespace Vizzu::Data
