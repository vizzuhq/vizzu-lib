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

bool operator<(const column_index_wrapper &lhs,
    const column_index_wrapper &rhs)
{
	return lhs.ncix.orig_index < rhs.ncix.orig_index;
}

bool operator==(const column_index_wrapper &lhs,
    const column_index_wrapper &rhs)
{
	return lhs.ncix.orig_index == rhs.ncix.orig_index;
}

data_table::cell_t::operator std::string() const
{
	return std::string{std::get<std::string_view>(cell)};
}

data_table::cell_t::operator double() const
{
	return std::get<double>(cell);
}

auto data_table::Row::operator[](column_index_wrapper colIx) const
    -> cell_t
{
	auto &&cell = rid.get_value(colIx.ncix.sid);

	if (double *d = std::get_if<double>(&cell); d && std::isnan(*d))
		*d = 0.0;

	return cell_t{cell};
}

std::size_t data_table::Row::size() const
{
	return rid.parent->get_dimensions().size()
	     + rid.parent->get_measures().size();
}

bool data_table::DataIndex::isInvalid() const
{
	return !value
	    || std::get<std::string_view>(value->ncix.sid).data()
	           == nullptr;
}

auto data_table::getColumn(const std::string &name) const
    -> column_index_wrapper
{
	auto sid = df.get_series_name(name);
	auto orig_index = df.get_series_orig_index(sid);

	return column_index_wrapper{{sid, orig_index}};
}
/*
auto data_table::column_info::categories() const
    -> std::span<const std::string>
{
    auto cats2 = dfif->get_categories(sid);

    if (dfif->has_na(sid)
        && std::ranges::find(cats2, "") == end(cats2)) {
        thread_local std::vector<std::string> th;
        th = {begin(cats2), end(cats2)};
        th.emplace_back("");
        cats2 = th;
    }
    return cats2;
}
*/
std::string data_table::column_info::getUnit() const
{
	return std::string{dfif->get_series_info(sid, "unit")};
}

std::string data_table::column_info::getName() const
{
	return std::string{dfif->get_series_name(sid)};
}

bool data_table::cell_wrapper::isDimension() const
{
	return std::holds_alternative<std::string_view>(cell);
}

const char *data_table::cell_wrapper::dimensionValue() const
{
	return std::get<std::string_view>(cell).data();
}

double data_table::cell_wrapper::operator*() const
{
	return std::get<double>(cell);
}

auto data_table::getRowCount() const -> std::size_t
{
	return df.get_record_count();
}

data_table::DataIndex data_table::getIndex(
    column_index_wrapper const &col) const
{
	return {DataIndex::OptColIndex{std::in_place, col.ncix},
	    df.get_series_type(col.ncix.sid)};
}

void data_table::addColumn(const std::string &name,
    const std::string &unit,
    const std::span<const double> &values)
{
	df.add_measure(values,
	    name.c_str(),
	    dataframe::adding_type::create_or_override,
	    {{std::pair{"unit", unit.c_str()}}});
}

void data_table::addColumn(const std::string &name,
    const std::span<const char *const> &categories,
    const std::span<const std::uint32_t> &values)
{
	df.add_dimension(categories,
	    values,
	    name.c_str(),
	    dataframe::adding_type::create_or_override,
	    {});

	df.remove_unused_categories(name);

	if (std::ranges::any_of(df.get_categories(name),
	        std::bind_front(std::equal_to{}, "")))
		df.fill_na(name, "");
}

void data_table::pushRow(const std::span<const char *> &cells)
{
	df.add_record(
	    std::vector<dataframe::cell_value>{begin(cells), end(cells)});

	for (const auto &dim : df.get_dimensions())
		if (std::ranges::any_of(df.get_categories(dim),
		        std::bind_front(std::equal_to{}, "")))
			df.fill_na(dim, "");
}

std::string data_table::getInfos() const { return df.as_string(); }

aggregator_t::operator double() const
{
	return my_res.value_or(double{});
}

auto series_type_t::aggregatorType() const
    -> dataframe::aggregator_type
{
	return aggr.value_or(dataframe::aggregator_type::exists);
}

bool series_type_t::isDimension() const { return !aggr; }

bool series_type_t::isReal() const
{
	const auto *sidp = std::get_if<std::string_view>(&sid);
	return sidp && sidp->data() != nullptr;
}

bool series_type_t::operator==(
    const dataframe::aggregator_type &aggr) const
{
	return aggregatorType() == aggr;
}

auto series_index_t::getType() const -> series_type_t
{
	return {cix.sid, aggr};
}

auto series_index_t::getColIndex() const -> OptColIndex
{
	return OptColIndex{std::in_place, cix};
}

bool operator==(const series_index_t &lhs, const series_index_t &rhs)
{
	return lhs.cix.sid == rhs.cix.sid && lhs.aggr == rhs.aggr;
}

bool operator<(const series_index_t &lhs, const series_index_t &rhs)
{
	return std::tuple{lhs.cix.orig_index, lhs.aggr}
	     < std::tuple{rhs.cix.orig_index, rhs.aggr};
}

bool slice_index_t::operator<(slice_index_t const &rhs) const
{
	return std::tuple{new_.orig_index, new_.orig_value}
	     < std::tuple{rhs.new_.orig_index, rhs.new_.orig_value};
}

bool slice_index_t::operator==(const slice_index_t &rhs) const
{
	return std::tuple{new_.orig_index, new_.orig_value}
	    == std::tuple{rhs.new_.orig_index, rhs.new_.orig_value};
}

bool multi_index_t::empty() const
{
	return parent->get_dimensions().empty();
}

bool data_cube_cell_t::isEmpty() const
{
	return !rid || parent->get_record_count() == 0
	    || parent->is_filtered(*rid);
}

bool data_cube_t::Id::operator==(const Id &id) const
{
	return std::tuple{itemSliceIndex, seriesId, itemId}
	    == std::tuple{id.itemSliceIndex, id.seriesId, id.itemId};
}

data_cube_t::Data::Iterator::Iterator(
    dataframe::dataframe_interface::record_type rid,
    const Data *parent,
    std::size_t old) :
    rid(rid),
    parent(parent),
    old(old)
{
	if (!old) incr();
}

void data_cube_t::Data::Iterator::incr()
{
	if (found) {
		++*std::get_if<std::size_t>(&rid.recordId);
		++found_count;
	}

	auto indices = parent->get_indices(old);

	found = std::ranges::all_of(
	    std::ranges::iota_view{std::size_t{}, indices.size()},
	    [&](std::size_t ix)
	    {
		    auto ixx = parent->dim_reindex[ix];
		    auto cats = rid.parent->get_categories({ixx});
		    if (cats.size() < indices[ix])
			    throw std::runtime_error("Wroong index?");

		    if (cats.size() == indices[ix]) {
			    return std::get<std::string_view>(
			               rid.get_value({ixx}))
			               .data()
			        == nullptr;
		    }

		    return cats[indices[ix]]
		        == std::get<std::string_view>(rid.get_value({ixx}));
	    });
}

bool data_cube_t::Data::Iterator::operator!=(
    const Iterator &oth) const
{
	return old != oth.old;
}

data_cube_t::Data::Iterator &data_cube_t::Data::Iterator::operator++()
{
	++old;

	incr();

	return *this;
}

data_cube_t::MultiIndex data_cube_t::Data::Iterator::getIndex() const
{
	return {rid.parent,
	    found ? std::make_optional(rid.recordId) : std::nullopt,
	    &parent->dim_reindex,
	    parent->get_indices(old)};
}

template <>
std::string series_index_t::toString(const DataTable &table) const
{
	std::string my_res;

	if (const auto *id = std::get_if<std::string_view>(&cix.sid);
	    !id || id->data() != nullptr) {
		my_res = table.getDf().get_series_name(cix.sid);
		if (aggr && *aggr != dataframe::aggregator_type::sum) {
			my_res = Refl::enum_name(*aggr) + "(" + std::move(my_res)
			       + ")";
		}
	}
	else {
		if (!aggr) throw std::runtime_error("Inconsistency...");
		my_res = Refl::enum_name(*aggr) + "()";
	}
	return my_res;
}

/*
template <>
series_index_t::series_index_t(
    SeriesType const &st)
{
    auto &&s = Refl::enum_name(st.aggregatorType());
    using CT = std::char_traits<char>;
    s[0] = CT::to_char_type(std::tolower(CT::to_int_type(s[0])));
    aggr = Refl::get_enum<dataframe::aggregator_type>(s);
    cix.sid = std::string_view{};
}
*/
template <>
series_index_t::series_index_t(
    DataTable::DataIndex const &dataIndex) :
    cix(dataIndex.value.value().ncix)
{
	if (dataIndex.type == DataTable::DataIndex::Type::measure)
		aggr = dataframe::aggregator_type::sum;
}

template <>
series_index_t::series_index_t(std::string const &str,
    const std::reference_wrapper<DataTable> &table)
{
	if (const Text::FuncString func(str, false);
	    !func.isEmpty()
	    && std::ranges::count(
	        Refl::get_names<dataframe::aggregator_type>(),
	        func.getName())) {
		aggr = Refl::get_enum<dataframe::aggregator_type>(
		    func.getName());
		if (func.getParams().empty()) {
			cix.sid = std::string_view{};
		}
		else {
			cix = table.get()
			          .getIndex(func.getParams().at(0))
			          .value.value()
			          .ncix;
		}
	}
	else {
		auto &&index = table.get().getIndex(str);
		cix = index.value.value().ncix;
		if (index.type == DataTable::DataIndex::Type::measure)
			aggr = dataframe::aggregator_type::sum;
	}
}

template <>
auto data_cube_t::Data::at(const MultiIndex &index) const
    -> data_cube_cell_t
{
	return {index.parent, index.rid};
}

std::vector<std::size_t> data_cube_t::Data::get_indices(
    std::size_t ix) const
{
	auto res = sizes;
	// reverse iterate, fill with the modulo, return with div
	for (auto &cur : std::ranges::views::reverse(res)) {
		if (!cur) continue;
		auto &&divres = std::lldiv(ix, cur);
		cur = static_cast<std::size_t>(divres.rem);
		ix = static_cast<std::size_t>(divres.quot);
	}
	return res;
}

auto data_cube_t::Data::begin() const -> Iterator
{
	return {{df, {std::size_t{}}}, this, std::size_t{}};
}

auto data_cube_t::Data::end() const -> Iterator
{
	return {{df, {df->get_record_count()}}, this, full_size};
}

template <>
data_cube_t::data_cube_t(const DataTable &table,
    const DataCubeOptions &options,
    const Filter &filter) :
    table(&table),
    df(options.getDimensions().empty()
                && options.getMeasures().empty()
            ? std::make_shared<dataframe::dataframe>()
            : table.getDf().copy(false, false)),
    data{df.get(), options}
{
	df->remove_records(
	    [filter, &table](
	        dataframe::dataframe_interface::record_type rec)
	    {
		    return !filter.match(
		        {table, table[std::get<std::size_t>(rec.recordId)]});
	    });

	removed = df->copy(false, false);

	for (const auto &dim : options.getDimensions())
		df->aggregate_by(dim.getColIndex().value().ncix.sid);

	for (const auto &meas : options.getMeasures()) {
		auto colIx = meas.getColIndex();
		auto sid = colIx ? colIx->ncix.sid : std::string_view{};
		auto aggr = meas.getAggr().value();

		measure_names.try_emplace(
		    std::pair{sid, Refl::enum_name(aggr)},
		    df->set_aggregate(sid, aggr));
	}

	if (options.getMeasures().empty()
	    && !options.getDimensions().empty()) {
		auto unkn =
		    dataframe::dataframe_interface::series_identifier{};
		measure_names.try_emplace(std::pair{unkn, "exists"},
		    df->set_aggregate(unkn,
		        dataframe::aggregator_type::exists));
	}

	for (const auto &dim : options.getDimensions()) {
		df->set_sort(dim.toString(table),
		    dataframe::sort_type::by_categories,
		    dataframe::na_position::last);
	}

	auto &&tr = std::ranges::transform_view(
	    std::ranges::iota_view{std::size_t{},
	        df->get_dimensions().size()},
	    [dims = df->get_dimensions()](std::size_t ix)
	        -> std::pair<std::string_view, std::size_t>
	    {
		    return {dims[ix], ix};
	    });

	df->finalize();

	for (auto it = data.dim_reindex.begin(),
	          sizIt = data.sizes.begin();
	     const auto &dim : options.getDimensions()) {
		auto &&name = dim.toString(table);
		*it++ =
		    (*std::ranges::lower_bound(tr,
		         std::pair<std::string_view, std::size_t>{name, {}}))
		        .second;
		*sizIt++ = df->get_categories(name).size() + df->has_na(name);
	}

	if (!options.getMeasures().empty()
	    || !options.getDimensions().empty())
		data.full_size = std::reduce(data.sizes.begin(),
		    data.sizes.end(),
		    std::size_t{1},
		    std::multiplies{});
}

template <>
size_t data_cube_t::combinedSizeOf(const SeriesList &colIndices) const
{
	std::size_t my_res{1};
	for (const auto &si : colIndices) {
		auto sid = si.getColIndex()->ncix.sid;
		my_res *= df->get_categories(sid).size() + df->has_na(sid);
	}
	return my_res;
}

size_t data_cube_t::subCellSize() const
{
	return df->get_measures().size();
}

template <>
std::string data_cube_t::getValue(const Id::SliceIndex &index,
    std::string def) const
{
	auto my_res = index.new_.value;
	if (my_res.data() == nullptr) {
		if (df->has_na(index.new_.column)) { my_res = ""; }
		else {
			my_res = def;
		}
	}
	return std::string{my_res};
}

template <>
data_cube_t::Id data_cube_t::getId(const SeriesList &sl,
    const MultiIndex &mi) const
{
	std::map<std::string_view, std::size_t> reindex;
	struct DimProp
	{
		std::string_view value;
		std::size_t index;
		std::size_t size;
		std::size_t orig_ix;
	};
	std::vector<std::pair<std::string_view, DimProp>> v;
	for (std::size_t ix{}; const auto &s : sl)
		reindex[v.emplace_back(std::get<std::string_view>(
		                           s.getColIndex().value().ncix.sid),
		             DimProp{})
		            .first] = ix++;

	std::size_t seriesId{};
	for (std::size_t ix{}; ix < mi.dim_reindex->size(); ++ix) {
		auto name =
		    mi.parent->get_series_name({(*mi.dim_reindex)[ix]});
		if (auto it = reindex.find(name); it != reindex.end()) {
			auto &&cats = mi.parent->get_categories(name);
			auto &[cat, cix, size, orig_ix] = v[it->second].second;
			orig_ix = ix;
			cix = mi.old[ix];
			size = cats.size() + mi.parent->has_na(name);
			if (cix < cats.size()) cat = cats[cix];
		}
		else {
			seriesId *=
			    df->get_categories(name).size() + df->has_na(name);
			seriesId += mi.old[ix];
		}
	}

	Id::SubSliceIndex ssi;
	std::size_t itemId{};
	for (std::size_t i{}; i < v.size(); ++i) {
		auto &[cat, cix, size, orig_ix] = v[i].second;

		ssi.push_back({{v[i].first, cat, orig_ix, cix}});
		itemId *= size;
		itemId += cix;
	}

	return {mi, ssi, seriesId, itemId};
}

template <>
auto data_cube_t::cellInfo(const MultiIndex &index) const -> CellInfo
{
	CellInfo my_res;

	for (std::size_t ix{}; ix < index.dim_reindex->size(); ++ix) {
		auto name =
		    index.parent->get_series_name({(*index.dim_reindex)[ix]});
		auto cats = index.parent->get_categories(name);
		auto cix = index.old[ix];
		my_res.categories[name] =
		    cix < cats.size() ? cats[cix] : std::string_view{};
	}

	if (index.rid)
		for (auto &&meas : df->get_measures()) {
			if (meas == "exists()") continue;
			my_res.values[meas] = std::get<double>(
			    index.parent->get_data(*index.rid, meas));
		}
	else
		for (auto &&meas : df->get_measures()) {
			if (meas == "exists()") continue;
			my_res.values[meas] = 0.0;
		}

	return my_res;
}

template <>
auto data_cube_t::valueAt(const MultiIndex &multiIndex,
    const SeriesIndex &seriesId) const -> aggregator_t
{
	if (multiIndex.rid) {
		auto colIx = seriesId.getColIndex();
		auto sid = colIx ? colIx->ncix.sid : std::string_view{};
		auto aggr = seriesId.getAggr().value();

		auto name = measure_names.at({sid, Refl::enum_name(aggr)});
		auto &&data = df->get_data(*multiIndex.rid, name);

		const auto *d = std::get_if<double>(&data);
		if (!d) throw std::runtime_error("valueAt not a double");

		return aggregator_t{*d};
	}

	return aggregator_t{std::nullopt};
}

template <>
auto data_cube_t::aggregateAt(const MultiIndex &multiIndex,
    const SeriesList &sumCols,
    SeriesIndex seriesId) const -> aggregator_t
{
	if (sumCols.empty()) return valueAt(multiIndex, seriesId);

	std::set<dataframe::dataframe_interface::series_identifier> ids;
	for (const auto &s : sumCols)
		ids.insert(s.getColIndex().value().ncix.sid);

	auto sid = seriesId.getColIndex()
	             ? seriesId.getColIndex()->ncix.sid
	             : std::string_view{};
	const auto agg = seriesId.getAggr().value();

	auto &name = measure_names.at({sid, Refl::enum_name(agg)});

	ids.insert(name);

	auto it = cacheImpl->find(ids);
	if (it == cacheImpl->end()) {
		it = cacheImpl->emplace(ids, removed->copy(false, false))
		         .first;

		auto &cp = it->second;

		auto keep_this =
		    std::set<std::string_view>(df->get_dimensions().begin(),
		        df->get_dimensions().end());
		for (const auto &dim : sumCols)
			if (keep_this.erase(df->get_series_name(
			        dim.getColIndex().value().ncix.sid))
			    != 1)
				throw std::runtime_error("Dimension was not here");

		for (const auto &dim : keep_this) cp->aggregate_by(dim);

		[[maybe_unused]] auto &&new_name =
		    cp->set_aggregate(sid, agg);
		if (new_name != name)
			throw std::runtime_error("Not matching names");

		for (auto &&dim : keep_this) {
			cp->set_sort(dim,
			    dataframe::sort_type::by_categories,
			    dataframe::na_position::last);
		}

		cp->finalize();
	}

	auto &sub_df = it->second;

	if (multiIndex.rid) {
		auto rrid = df->get_record_id_by_dims(*multiIndex.rid,
		    sub_df->get_dimensions());

		auto my_res = std::get<double>(sub_df->get_data(rrid, name));

		return aggregator_t{my_res};
	}

	std::map<std::string_view, std::size_t> index;

	for (std::size_t ix{}; ix < multiIndex.dim_reindex->size(); ++ix)
		index.emplace(
		    df->get_series_name((*multiIndex.dim_reindex)[ix]),
		    multiIndex.old[ix]);

	struct comp
	{
		std::shared_ptr<dataframe::dataframe_interface> &sub_df;
		bool operator()(std::size_t iTh,
		    const std::map<std::string_view, std::size_t> &index)
		    const
		{
			for (const auto &dim : sub_df->get_dimensions()) {
				auto cat = std::get<std::string_view>(
				    sub_df->get_data(iTh, dim));
				auto &&cats = sub_df->get_categories(dim);
				auto &&refIx = index.at(dim);
				if (refIx == cats.size()) {
					if (cat.data() != nullptr) return true;
					continue;
				}
				if (cat == cats[refIx]) continue;
				auto until = cats.begin() + refIx;
				return std::find(cats.begin(), until, cat) != until;
			}
			return false;
		}

		bool operator()(
		    const std::map<std::string_view, std::size_t> &index,
		    std::size_t iTh) const
		{
			for (const auto &dim : sub_df->get_dimensions()) {
				auto cat = std::get<std::string_view>(
				    sub_df->get_data(iTh, dim));
				auto &&cats = sub_df->get_categories(dim);
				auto &&refIx = index.at(dim);
				if (refIx == cats.size()) {
					if (cat.data() != nullptr) return false;
					continue;
				}
				if (cat == cats[refIx]) continue;
				auto until = cats.begin() + refIx;
				return std::find(cats.begin(), until, cat) == until;
			}
			return false;
		}
	};

	std::ranges::iota_view rix{std::size_t{},
	    sub_df->get_record_count()};

	auto [beg, end] =
	    std::equal_range(rix.begin(), rix.end(), index, comp{sub_df});

	std::optional<double> my_res =
	    beg == end ? std::nullopt
	               : std::make_optional(std::get<double>(
	                   sub_df->get_data(*beg, name)));

	return aggregator_t{my_res};
}

} // namespace Vizzu::Data
