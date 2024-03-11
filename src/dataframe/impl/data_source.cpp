
#include "data_source.h"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <utility>
#include <vector>

#include "base/text/naturalcmp.h"

#include "dataframe.h"

namespace Vizzu::dataframe
{

template <bool /*stable*/ = false, bool /*sparse*/ = true>
struct index_erase_if
{
	std::span<const std::size_t> indices;

	template <class Cont>
	constexpr void operator()(Cont &&cont) const noexcept;
};

template <>
template <class Cont>
constexpr void index_erase_if<true>::operator()(
    Cont &&cont) const noexcept
{
	if (indices.empty()) return;
	auto into = cont.begin() + indices.front();
	auto prev = into + 1;

	for (std::size_t i{1}; i < indices.size(); ++i) {
		auto curr = cont.begin() + indices[i];
		into = std::move(std::exchange(prev, curr + 1), curr, into);
	}
	cont.erase(std::move(prev, cont.end(), into), cont.end());
}

template <>
template <class Cont>
constexpr void index_erase_if<>::operator()(
    Cont &&cont) const noexcept
{
	auto from = cont.end();
	for (auto i : std::ranges::views::reverse(indices))
		cont[i] = std::move(*--from);
	cont.erase(from, cont.end());
}

template <>
template <class Cont>
constexpr void index_erase_if<true, false>::operator()(
    Cont &&cont) const noexcept
{
	std::erase_if(cont,
	    [i = std::size_t{}, c = indices.begin(), e = indices.end()](
	        const auto &) mutable
	    {
		    return c != e && i++ == *c && (++c, true);
	    });
}

template <>
template <class Cont>
constexpr void index_erase_if<false, false>::operator()(
    Cont &&cont) const noexcept
{
	auto from_ix = cont.size();
	for (auto first = begin(indices),
	          last = std::ranges::lower_bound(indices,
	              from_ix - indices.size()),
	          from = indices.end();
	     first != last;
	     cont[*first++] = std::move(cont[from_ix]))
		while (--from_ix == from[-1]) --from;
}

std::size_t data_source::get_record_count() const
{
	if (finalized) return finalized->record_unique_ids.size();

	std::size_t record_count{};
	for (const auto &dim : dimensions)
		if (record_count < dim.values.size())
			record_count = dim.values.size();

	for (const auto &mea : measures)
		if (record_count < mea.values.size())
			record_count = mea.values.size();

	return record_count;
}

struct data_source::sorter
{
	[[nodiscard]] static std::weak_ordering
	cmp_dim(std::uint32_t lhs, std::uint32_t rhs, na_position na)
	{
		return na == na_position::last || (lhs != nav && rhs != nav)
		         ? lhs <=> rhs
		         : rhs <=> lhs;
	}

	[[nodiscard]] static std::weak_ordering
	cmp_mea(double lhs, double rhs, na_position na, sort_type sort)
	{
		using std::weak_order;
		return std::isnan(lhs) || std::isnan(rhs)
		         ? na == na_position::last ? weak_order(lhs, rhs)
		                                   : weak_order(rhs, lhs)
		     : sort == sort_type::less ? weak_order(lhs, rhs)
		                               : weak_order(rhs, lhs);
	}

	[[nodiscard]] static std::weak_ordering
	cmp(const sort_one_series &sorter, std::size_t a, std::size_t b)
	{
		switch (auto &&[series, sort, na] = sorter; series) {
			using enum series_type;
		case dimension: {
			const auto &dim = unsafe_get<dimension>(series).second;
			return cmp_dim(dim.values[a], dim.values[b], na);
		}
		case measure: {
			const auto &mea = unsafe_get<measure>(series).second;
			return cmp_mea(mea.values[a], mea.values[b], na, sort);
		}
		default:
		case unknown: return std::weak_ordering::equivalent;
		}
	}
};

std::vector<std::size_t> data_source::get_sorted_indices(
    const dataframe_interface *parent,
    const sorting_type &sorters) const
{
	std::vector<std::size_t> result(get_record_count());
	std::iota(result.begin(), result.end(), 0);
	std::sort(result.begin(),
	    result.end(),
	    [parent, &sorters](std::size_t a, std::size_t b)
	    {
		    for (const auto &sorter : sorters) {
			    if (const auto *custom = std::get_if<1>(&sorter)) {
				    if (auto res =
				            (*custom)({parent, a}, {parent, b});
				        is_neq(res))
					    return is_lt(res);
			    }
			    else if (auto res =
			                 sorter::cmp(*std::get_if<0>(&sorter),
			                     a,
			                     b);
			             is_neq(res))
				    return is_lt(res);
		    }
		    return false;
	    });
	return result;
}

void data_source::sort(std::vector<std::size_t> &&indices)
{
	std::vector<double> tmp_mea(measures.size());
	std::vector<std::uint32_t> tmp_dim(dimensions.size());

	for (std::size_t i{}, max = indices.size(); i < max; ++i) {
		if (i >= indices[i]) continue;

		for (std::size_t m{}; m < measures.size(); ++m)
			tmp_mea[m] = measures[m].values[i];
		for (std::size_t d{}; d < dimensions.size(); ++d)
			tmp_dim[d] = dimensions[d].values[i];

		std::size_t j{i};
		for (; indices[j] != i; j = std::exchange(indices[j], i)) {
			for (auto &meas : measures)
				meas.values[j] = meas.values[indices[j]];
			for (auto &dims : dimensions)
				dims.values[j] = dims.values[indices[j]];
		}
		for (std::size_t m{}; m < measures.size(); ++m)
			measures[m].values[j] = tmp_mea[m];
		for (std::size_t d{}; d < dimensions.size(); ++d)
			dimensions[d].values[j] = tmp_dim[d];
	}
}

void data_source::change_series_identifier_type(
    series_identifier &id) const
{
	if (const auto *size = std::get_if<std::size_t>(&id)) {
		if (*size < dimension_names.size())
			id = dimension_names[*size];
		else if (*size
		         < dimension_names.size() + measure_names.size())
			id = measure_names[*size - dimension_names.size()];
		else
			id = std::string_view{};
	}
	else {
		auto name = *std::get_if<std::string_view>(&id);
		if (finalized)
			if (auto it = finalized->series_to_index.find(name);
			    it != finalized->series_to_index.end())
				id = it->second;

		if (auto it = std::lower_bound(dimension_names.begin(),
		        dimension_names.end(),
		        name);
		    it != dimension_names.end() && *it == name)
			id = static_cast<std::size_t>(
			    it - dimension_names.begin());
		else if (it = std::lower_bound(measure_names.begin(),
		             measure_names.end(),
		             name);
		         it != measure_names.end() && *it == name)
			id = static_cast<std::size_t>(
			         std::distance(measure_names.begin(), it))
			   + dimension_names.size();
		else
			id = ~std::size_t{};
	}
}

void data_source::change_record_identifier_type(
    record_identifier &id) const
{
	if (!finalized) throw std::runtime_error("not finalized yet");

	if (const auto *size = std::get_if<std::size_t>(&id)) {
		if (*size < finalized->record_unique_ids.size())
			id = finalized->record_unique_ids[*size];
		else
			id = std::string_view{};
	}
	else {
		auto name = *std::get_if<std::string_view>(&id);
		if (auto it = finalized->record_to_index.find(name);
		    it != finalized->record_to_index.end())
			id = it->second;
		else
			id = ~std::size_t{};
	}
}

cell_value data_source::get_data(std::size_t record_id,
    const series_identifier &column) const
{
	switch (auto &&series = get_series(column)) {
		using enum series_type;
	case dimension: {
		const auto &dims = unsafe_get<dimension>(series).second;
		if (record_id >= dims.values.size())
			return std::string_view{};
		return dims.get(record_id);
	}
	case measure: {
		const auto &meas = unsafe_get<measure>(series).second;
		if (record_id >= meas.values.size()) return nan;

		return meas.values[record_id];
	}
	case unknown:
	default: throw std::runtime_error("unknown column");
	}
}

data_source::series_data data_source::get_series(series_identifier id)
{
	if (std::holds_alternative<std::string_view>(id))
		change_series_identifier_type(id);
	auto size = *get_if<std::size_t>(&id);

	if (size < dimensions.size())
		return series_data{std::in_place_index<1>,
		    dimension_names[size],
		    dimensions[size]};
	if (size < dimensions.size() + measures.size())
		return series_data{std::in_place_index<2>,
		    measure_names[size - dimensions.size()],
		    measures[size - dimensions.size()]};
	return {};
}

data_source::const_series_data data_source::get_series(
    series_identifier id) const
{
	if (std::holds_alternative<std::string_view>(id))
		change_series_identifier_type(id);
	auto size = *std::get_if<std::size_t>(&id);

	if (size < dimensions.size())
		return const_series_data{std::in_place_index<1>,
		    dimension_names[size],
		    dimensions[size]};
	if (size < dimensions.size() + measures.size())
		return const_series_data{std::in_place_index<2>,
		    measure_names[size - dimensions.size()],
		    measures[size - dimensions.size()]};

	return {};
}

void data_source::normalize_sizes()
{
	std::size_t record_count{};
	for (auto &&dim : dimensions)
		record_count = std::max(record_count, dim.values.size());
	for (auto &&mea : measures)
		record_count = std::max(record_count, mea.values.size());

	for (auto &&dim : dimensions)
		dim.values.resize(record_count, nav);
	for (auto &&mea : measures) mea.values.resize(record_count, nan);
}

void data_source::remove_series(std::span<const std::size_t> dims,
    std::span<const std::size_t> meas)
{
	const index_erase_if<true> dim_remover{dims};
	dim_remover(dimensions);
	dim_remover(dimension_names);
	const index_erase_if<true> mea_remover{meas};
	mea_remover(measures);
	mea_remover(measure_names);
}

std::pair<double, double> data_source::get_min_max(
    const measure_t &measure) const
{
	return finalized ? finalized->min_max[&measure - measures.data()]
	                 : measure.get_min_max();
}

data_source::dimension_t &data_source::add_new_dimension(
    std::span<const char *const> dimension_categories,
    std::span<const std::uint32_t> dimension_values,
    std::string_view name,
    std::span<const std::pair<const char *, const char *>> info)
{
	auto it = dimension_names.emplace(
	    std::lower_bound(dimension_names.begin(),
	        dimension_names.end(),
	        name),
	    name);
	return *dimensions.emplace(dimensions.begin()
	                               + (it - dimension_names.begin()),
	    dimension_categories,
	    dimension_values,
	    info);
}

data_source::dimension_t &data_source::add_new_dimension(
    dimension_t &&dim,
    std::string_view name)
{
	auto it = dimension_names.emplace(
	    std::lower_bound(dimension_names.begin(),
	        dimension_names.end(),
	        name),
	    name);
	return *dimensions.emplace(dimensions.begin()
	                               + (it - dimension_names.begin()),
	    std::move(dim));
}

data_source::measure_t &data_source::add_new_measure(
    std::span<const double> measure_values,
    std::string_view name,
    std::span<const std::pair<const char *, const char *>> info)
{
	auto it =
	    measure_names.emplace(std::lower_bound(measure_names.begin(),
	                              measure_names.end(),
	                              name),
	        name);
	return *measures.emplace(measures.begin()
	                             + (it - measure_names.begin()),
	    measure_values,
	    info);
}

data_source::measure_t &data_source::add_new_measure(measure_t &&mea,
    std::string_view name)
{
	auto it =
	    measure_names.emplace(std::lower_bound(measure_names.begin(),
	                              measure_names.end(),
	                              name),
	        name);
	return *measures.emplace(measures.begin()
	                             + (it - measure_names.begin()),
	    std::move(mea));
}

void data_source::remove_records(std::span<const std::size_t> indices)
{
	const index_erase_if<false> indices_remover{indices};
	for (auto &&dim : dimensions) indices_remover(dim.values);
	for (auto &&mea : measures) indices_remover(mea.values);
}

struct aggregating_helper
{
	std::size_t index;
	std::vector<custom_aggregator::id_type> aggregators;
};

data_source::data_source(aggregating_type &&aggregating,
    const std::vector<bool> *filtered,
    std::size_t record_count)
{
	auto &[dims, meas] = aggregating;

	dimensions.reserve(dims.size());
	dimension_names.reserve(dims.size());
	measures.reserve(meas.size());
	measure_names.reserve(meas.size());

	for (const auto &[name, dim] : dims) {
		dimension_t &new_dim = add_new_dimension({}, name);
		new_dim.categories = dim.get().categories;
		new_dim.info = dim.get().info;
	}

	for (const auto &[name, mea] : meas) {
		measure_t &new_mea = add_new_measure({}, name);
		switch (auto &ser = std::get<0>(mea)) {
			using enum series_type;
		case dimension:
			new_mea.info = unsafe_get<dimension>(ser).second.info;
			break;
		case measure:
			new_mea.info = unsafe_get<measure>(ser).second.info;
			break;
		case unknown:
		default: break;
		}
	}

	std::vector<std::uint32_t> cat_indices(dims.size());
	std::map<std::vector<std::uint32_t>, aggregating_helper>
	    rec_to_index;

	for (std::size_t i{}; i < record_count; ++i) {
		if (filtered && (*filtered)[i]) continue;

		for (std::size_t ix{}; const auto &[name, dim] : dims)
			cat_indices[ix++] = dim.get().values[i];

		auto [it, s] = rec_to_index.try_emplace(cat_indices,
		    rec_to_index.size());

		auto &[index, aggregators] = it->second;
		if (s) {
			for (auto &m : measures) m.values.emplace_back(NAN);

			for (std::size_t ix{}; const auto &[name, dim] : dims)
				dimensions[ix++].values.emplace_back(
				    dim.get().values[i]);

			aggregators.reserve(meas.size());
			for (const auto &[ser, agg] :
			    std::ranges::views::values(meas))
				aggregators.emplace_back(agg.create());
		}

		for (std::size_t ix{}; const auto &[name, mea] : meas) {
			auto &[data, agg] = mea;
			cell_value val{};
			if (data == series_type::measure)
				val = unsafe_get<series_type::measure>(data)
				          .second.values[i];
			else
				val = unsafe_get<series_type::dimension>(data)
				          .second.get(i);
			measures[ix].values[index] =
			    agg.add(aggregators[ix], val);
			++ix;
		}
	}
}

data_source::data_source(
    const std::shared_ptr<const data_source> &copying,
    std::optional<std::vector<bool>> &&filtered,
    std::optional<std::vector<std::size_t>> &&sorted) :
    measure_names(copying->measure_names),
    measures(copying->measures),
    dimension_names(copying->dimension_names),
    dimensions(copying->dimensions)
{
	if (sorted) { this->sort(std::move(*sorted)); }
	if (filtered) {
		const auto &filt = *filtered;
		std::vector<std::size_t> remove_ix;
		remove_ix.reserve(filt.size());
		for (std::size_t i{}; i < filt.size(); ++i)
			if (filt[i]) remove_ix.emplace_back(i);
		remove_records(remove_ix);
	}
}

data_source::final_info::final_info(const data_source &source) :
    min_max(source.measure_names.size()),
    record_unique_ids(source.get_record_count())
{
	const auto measures = source.measure_names.size();
	const auto dimensions = source.dimension_names.size();
	const auto records = record_unique_ids.size();

	series_to_index.reserve(measures + dimensions);
	record_to_index.reserve(records);

	for (std::size_t i{}; i < dimensions; ++i)
		series_to_index[source.dimension_names[i]] = i;

	for (std::size_t i{}; i < measures; ++i) {
		series_to_index[source.measure_names[i]] = i + dimensions;
		min_max[i] = source.measures[i].get_min_max();
	}

	for (std::size_t r{}; r < records; ++r) {
		auto &record = record_unique_ids[r];

		for (std::size_t d{}; d < dimensions; ++d) {
			auto val = source.dimensions[d].values[r];
			record +=
			    source.dimension_names[d] + ':'
			    + (val == nav ? "null"
			                  : source.dimensions[d].categories[val])
			    + ";";
		}
		if (!record_to_index.try_emplace(record, r).second)
			throw std::runtime_error("duplicated record");
	}
}

std::vector<std::uint32_t> data_source::dimension_t::get_indices(
    const dataframe_interface::any_sort_type &sorter) const
{
	std::vector<std::uint32_t> indices(categories.size());
	std::iota(indices.begin(), indices.end(), 0);
	std::sort(indices.begin(),
	    indices.end(),
	    [this, &sorter, s = std::get_if<sort_type>(&sorter)](
	        std::size_t a,
	        std::size_t b)
	    {
		    static const Text::NaturalCmp cmp{};
		    if (s) switch (*s) {
			    default:
			    case sort_type::less:
				    return categories[a] < categories[b];
			    case sort_type::greater:
				    return categories[b] < categories[a];
			    case sort_type::natural_less:
				    return cmp(categories[a], categories[b]);
			    case sort_type::natural_greater:
				    return cmp(categories[b], categories[a]);
			    }
		    else
			    return std::is_lt((*std::get_if<1>(
			        &sorter))(categories[a], categories[b]));
	    });

	return indices;
}

void data_source::dimension_t::sort_by(
    std::vector<std::uint32_t> &&indices,
    na_position na)
{
	for (auto &val : values)
		if (val != nav) val = indices[val];

	for (std::size_t i{}; i < categories.size(); ++i) {
		if (i >= indices[i]) continue;

		auto tmp = std::move(categories[i]);

		std::size_t j{i};
		for (; indices[j] != i; j = std::exchange(indices[j], i))
			categories[j] = std::move(categories[indices[j]]);

		categories[j] = std::move(tmp);
	}
	na_pos = na;
}

void data_source::dimension_t::add_element(
    std::string_view const &cat)
{
	values.emplace_back(get_or_set_cat(cat));
}
void data_source::dimension_t::add_more_data(
    std::span<const char *const> new_categories,
    std::span<const std::uint32_t> new_values)
{
	std::vector<std::uint32_t> remap(new_categories.size());
	for (auto i = std::size_t{}; i < remap.size(); ++i) {
		remap[i] = get_or_set_cat(new_categories[i]);
	}

	for (const auto val : new_values) values.emplace_back(remap[val]);
}
std::string_view data_source::dimension_t::get(
    std::size_t index) const
{
	return values[index] == data_source::nav
	         ? std::string_view{}
	         : categories[values[index]];
}

void data_source::dimension_t::set(std::size_t index,
    std::string_view value)
{
	values[index] =
	    value.data() ? get_or_set_cat(value) : data_source::nav;
}
void data_source::dimension_t::set_nav(std::string_view value)
{
	if (value.data() == nullptr) value = "";

	auto ix = get_or_set_cat(value);
	for (auto &val : values)
		if (val == data_source::nav) val = ix;
}
std::uint32_t data_source::dimension_t::get_or_set_cat(
    std::string_view cat)
{
	if (cat.data() == nullptr) return nav;
	auto it = std::find(categories.begin(), categories.end(), cat);
	if (it == categories.end())
		it = categories.emplace(categories.end(), cat);
	return static_cast<std::uint32_t>(it - categories.begin());
}

std::pair<double, double> data_source::measure_t::get_min_max() const
{
	auto mini = std::numeric_limits<double>::max();
	auto maxi = std::numeric_limits<double>::lowest();

	for (auto val : values) {
		if (std::isinf(val) || std::isnan(val)) continue;
		if (val < mini) mini = val;
		if (val > maxi) maxi = val;
	}
	return {mini, maxi};
}

std::string data_source::aggregating_type::add_aggregated(
    const_series_data &&data,
    const custom_aggregator &aggregator)
{
	return meas
	    .try_emplace(
	        std::string{aggregator.get_name()} + '('
	            + std::visit(
	                [](const auto &arg)
	                {
		                if constexpr (std::is_same_v<std::monostate,
		                                  std::remove_cvref_t<
		                                      decltype(arg)>>)
			                return std::string{};
		                else
			                return std::string{arg.first};
	                },
	                data)
	            + ')',
	        data,
	        aggregator)
	    .first->first;
}

}