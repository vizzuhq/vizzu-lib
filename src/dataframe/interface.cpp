#include "interface.h"

#include <compare>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <new>
#include <span>
#include <string>
#include <string_view>
#include <utility>

#include "base/text/immutable_string.h"
#include "impl/dataframe.h"

namespace Vizzu::dataframe
{

static_assert(sizeof(dataframe) <= max_size_impl);
static_assert(alignof(dataframe) <= align_impl);

dataframe &as_impl(dataframe_interface *df)
{
	// NOLINTBEGIN(bugprone-casting-through-void)
	return *std::launder(
	    static_cast<dataframe *>(static_cast<void *>(df->data)));
	// NOLINTEND(bugprone-casting-through-void)
}

const dataframe &as_impl(const dataframe_interface *df)
{
	// NOLINTBEGIN(bugprone-casting-through-void)
	return *std::launder(static_cast<const dataframe *>(
	    static_cast<const void *>(df->data)));
	// NOLINTEND(bugprone-casting-through-void)
}

std::shared_ptr<dataframe_interface> dataframe_interface::copy(
    bool inherit_sorting) const &
{
	return as_impl(this).copy(inherit_sorting);
}

Text::immutable_string dataframe_interface::set_aggregate(
    const Text::immutable_string &series,
    const any_aggregator_type &aggregator) &
{
	return as_impl(this).set_aggregate(series, aggregator);
}

void dataframe_interface::set_sort(
    const Text::immutable_string &series,
    any_sort_type sort,
    na_position na_pos) &
{
	as_impl(this).set_sort(series, sort, na_pos);
}

void dataframe_interface::set_custom_sort(
    const std::function<std::weak_ordering(record_type, record_type)>
        &custom_sort) &
{
	as_impl(this).set_custom_sort(custom_sort);
}

const Text::immutable_string &dataframe_interface::add_dimension(
    std::span<const char *const> dimension_categories,
    std::span<const std::uint32_t> dimension_values,
    std::string_view name,
    adding_type adding_strategy,
    std::span<const std::pair<const char *, const char *>> info) &
{
	return as_impl(this).add_dimension(dimension_categories,
	    dimension_values,
	    name,
	    adding_strategy,
	    info);
}

const Text::immutable_string &dataframe_interface::add_measure(
    std::span<const double> measure_values,
    std::string_view name,
    adding_type adding_strategy,
    std::span<const std::pair<const char *, const char *>> info) &
{
	return as_impl(this).add_measure(measure_values,
	    name,
	    adding_strategy,
	    info);
}

const Text::immutable_string &
dataframe_interface::add_series_by_other(std::string_view curr_series,
    std::string_view name,
    const std::function<cell_value(record_type, cell_reference)>
        &value_transform,
    std::span<const std::pair<const char *, const char *>> info) &
{
	return as_impl(this).add_series_by_other(curr_series,
	    name,
	    value_transform,
	    info);
}

void dataframe_interface::remove_series(
    std::span<const std::string_view> names) &
{
	as_impl(this).remove_series(names);
}

void dataframe_interface::add_record(
    std::span<const char *const> values) &
{
	as_impl(this).add_record(values);
}

void dataframe_interface::remove_records(
    const std::function<bool(const record_type &)> &filter) &
{
	as_impl(this).remove_records(filter);
}

void dataframe_interface::remove_unused_categories(
    std::string_view column) &
{
	as_impl(this).remove_unused_categories(column);
}

void dataframe_interface::change_data(std::size_t record_id,
    std::string_view column,
    cell_value value) &
{
	as_impl(this).change_data(record_id, column, value);
}

bool dataframe_interface::has_na(
    const std::string_view &column) const &
{
	return as_impl(this).has_na(column);
}

void dataframe_interface::fill_na(std::string_view column,
    cell_value value) &
{
	as_impl(this).fill_na(column, value);
}

void dataframe_interface::finalize() & { as_impl(this).finalize(); }

std::span<const Text::immutable_string>
dataframe_interface::get_dimensions() const &
{
	return as_impl(this).get_dimensions();
}

std::span<const Text::immutable_string>
dataframe_interface::get_measures() const &
{
	return as_impl(this).get_measures();
}

std::span<const Text::immutable_string>
dataframe_interface::get_categories(
    const std::string_view &dimension) const &
{
	return as_impl(this).get_categories(dimension);
}

cell_reference dataframe_interface::get_data(
    const record_identifier &record_id,
    const std::string_view &column) const &
{
	return as_impl(this).get_data(record_id, column);
}

std::size_t dataframe_interface::get_record_count() const &
{
	return as_impl(this).get_record_count();
}

Text::immutable_string dataframe_interface::get_series_info(
    const Text::immutable_string &id,
    const char *key) const &
{
	return as_impl(this).get_series_info(id, key);
}

bool dataframe_interface::is_removed(std::size_t record_id) const &
{
	return as_impl(this).is_removed(record_id);
}

Text::immutable_string dataframe_interface::get_record_id_by_dims(
    std::size_t my_record,
    std::span<const Text::immutable_string> dimensions) const &
{
	return as_impl(this).get_record_id_by_dims(my_record, dimensions);
}

Text::immutable_string dataframe_interface::get_record_id(
    std::size_t my_record) &
{
	return as_impl(this).get_record_id(my_record);
}

}