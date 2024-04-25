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

std::string_view data_table::getUnit(
    std::string_view const &colIx) const
{
	return df.get_series_info(colIx, "unit");
}

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

bool data_cube_t::iterator_t::operator!=(const iterator_t &oth) const
{
	return parent != oth.parent;
}

void data_cube_t::iterator_t::operator++() { parent->incr(*this); }

const data_cube_t::multi_index_t &
data_cube_t::iterator_t::operator*() const
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

data_cube_t::iterator_t data_cube_t::begin() const
{
	if (df->get_record_count() == 0) return {};
	iterator_t res{this,
	    {},
	    {{}, std::vector<std::size_t>(dim_reindex.size())}};
	check(res);
	return res;
}

data_cube_t::iterator_t data_cube_t::end() { return {}; }

void data_cube_t::check(iterator_t &it) const
{
	if (it.index.rid) {
		++it.rid;
		it.index.rid.reset();
	}

	for (std::size_t ix{}; ix < dim_reindex.size(); ++ix) {
		auto &&dim = dim_reindex[ix].first;
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

void data_cube_t::incr(iterator_t &it) const
{
	for (std::size_t ix{dim_reindex.size()}; ix-- > 0;)
		if (++it.index.old[ix] >= dim_reindex[ix].second)
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
    df(options.getDimensions().empty()
                && options.getMeasures().empty()
            ? dataframe::dataframe::create_new()
            : table.getDf().copy(false)),
    dim_reindex(options.getDimensions().size())
{
	df->remove_records(filter.getFunction());

	removed = df->copy(false);

	for (const auto &dim : options.getDimensions())
		df->aggregate_by(dim.getColIndex());

	for (const auto &meas : options.getMeasures()) {
		auto &&sid = meas.getColIndex();
		auto &&aggr = meas.getAggr();

		measure_names.try_emplace(std::pair{sid, aggr},
		    df->set_aggregate(sid, aggr));
	}

	for (const auto &dim : options.getDimensions()) {
		df->set_sort(dim.getColIndex(),
		    dataframe::sort_type::by_categories,
		    dataframe::na_position::last);
	}

	df->finalize();

	for (auto it = dim_reindex.begin();
	     const auto &dim : options.getDimensions()) {
		auto &&dimName = dim.getColIndex();
		*it++ = {*std::lower_bound(df->get_dimensions().begin(),
		             df->get_dimensions().end(),
		             dimName),
		    df->get_categories(dimName).size() + df->has_na(dimName)};
	}
}

size_t data_cube_t::combinedSizeOf(
    const series_index_list_t &colIndices) const
{
	std::size_t my_res{1};
	for (const auto &si : colIndices) {
		auto sid = si.getColIndex();
		my_res *= df->get_categories(sid).size() + df->has_na(sid);
	}
	return my_res;
}

bool data_cube_t::empty() const
{
	return df->get_measures().empty() && df->get_dimensions().empty();
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

std::string_view data_cube_t::getUnit(
    std::string_view const &colIx) const
{
	return df->get_series_info(colIx, "unit");
}

data_cube_t::Id data_cube_t::getId(const series_index_list_t &sl,
    const multi_index_t &mi) const
{
	std::map<std::string_view, std::size_t> reindex;
	std::vector<std::pair<std::size_t, std::size_t>> v(sl.size());
	Id::SubSliceIndex ssi(sl.size());

	for (std::size_t ix{}; const auto &s : sl)
		reindex.try_emplace(s.getColIndex(), ix++);

	std::size_t seriesId{};
	for (std::size_t ix{}; ix < dim_reindex.size(); ++ix) {
		auto &&[name, size] = dim_reindex[ix];
		if (auto it = reindex.find(name); it != reindex.end()) {
			v[it->second] = {mi.old[ix], size};
			auto &&cats = df->get_categories(name);
			ssi[it->second] = {name,
			    mi.old[ix] < cats.size() ? cats[mi.old[ix]]
			                             : std::string_view{}};
		}
		else
			seriesId = seriesId * size + mi.old[ix];
	}

	std::size_t itemId{};
	for (const auto &[cix, size] : v) itemId = itemId * size + cix;

	return {mi, ssi, seriesId, itemId};
}

CellInfo data_cube_t::cellInfo(const multi_index_t &index) const
{
	CellInfo my_res;

	for (std::size_t ix{}; ix < dim_reindex.size(); ++ix) {
		auto &&name = dim_reindex[ix].first;
		auto cats = df->get_categories(name);
		auto cix = index.old[ix];
		my_res.categories.emplace_back(name,
		    cix < cats.size() ? cats[cix] : std::string_view{});
	}

	for (auto &&meas : df->get_measures())
		my_res.values.emplace_back(meas,
		    index.rid
		        ? std::get<double>(df->get_data(*index.rid, meas))
		        : 0.0);

	return my_res;
}

double data_cube_t::valueAt(const multi_index_t &multiIndex,
    const series_index_t &seriesId) const
{
	if (multiIndex.rid) {
		const auto &name = getName(seriesId);
		return std::get<double>(df->get_data(*multiIndex.rid, name));
	}

	return {};
}

double data_cube_t::aggregateAt(const multi_index_t &multiIndex,
    const series_index_list_t &sumCols,
    const series_index_t &seriesId) const
{
	if (sumCols.empty()) return valueAt(multiIndex, seriesId);

	std::set<std::string> id{};
	for (const auto &dim : sumCols) id.emplace(dim.getColIndex());

	const auto &name = getName(seriesId);

	id.emplace(name);

	auto it = cacheImpl->find(id);
	if (it == cacheImpl->end()) {
		auto &[idset, cp] =
		    *(it = cacheImpl
		               ->emplace(std::move(id), removed->copy(false))
		               .first);

		struct It
		{
			std::shared_ptr<dataframe::dataframe_interface> &df;
			It &operator++() { return *this; }
			It &operator*() { return *this; }
			It &operator=(const std::string &str)
			{
				df->aggregate_by(str);
				return *this;
			}
		};

		std::set_difference(df->get_dimensions().begin(),
		    df->get_dimensions().end(),
		    idset.begin(),
		    idset.end(),
		    It{cp});

		[[maybe_unused]] auto &&new_name =
		    cp->set_aggregate(seriesId.getColIndex(),
		        seriesId.getAggr());

		cp->finalize();
	}

	auto &sub_df = *it->second;

	if (multiIndex.rid) {
		auto &&rrid = df->get_record_id_by_dims(*multiIndex.rid,
		    sub_df.get_dimensions());
		return std::get<double>(sub_df.get_data(rrid, name));
	}

	// hack for sunburst.
	std::map<std::string_view, std::size_t> index;

	for (std::size_t ix{}; ix < dim_reindex.size(); ++ix)
		index.emplace(dim_reindex[ix].first, multiIndex.old[ix]);

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
