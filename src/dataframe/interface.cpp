#include "interface.h"

#include <memory>
#include <new>

#include "impl/dataframe.h"

namespace Vizzu::dataframe
{

static_assert(sizeof(dataframe) <= max_size_impl);
static_assert(alignof(dataframe) <= align_impl);

dataframe &as_impl(dataframe_interface *df)
{
	return *std::launder(
	    static_cast<dataframe *>(static_cast<void *>(df->data)));
}

const dataframe &as_impl(const dataframe_interface *df)
{
	return *std::launder(static_cast<const dataframe *>(
	    static_cast<const void *>(df->data)));
}

std::shared_ptr<dataframe_interface> dataframe_interface::copy(
    bool inherit_sorting) const &
{
	return as_impl(this).copy(inherit_sorting);
}

std::string dataframe_interface::set_aggregate(
    std::string_view series,
    const any_aggregator_type &aggregator) &
{
	return as_impl(this).set_aggregate(series, aggregator);
}

void dataframe_interface::set_sort(std::string_view series,
    any_sort_type sort,
    na_position na_pos) &
{
	as_impl(this).set_sort(series, std::move(sort), na_pos);
}

void dataframe_interface::set_custom_sort(
    std::function<std::weak_ordering(record_type, record_type)>
        custom_sort) &
{
	as_impl(this).set_custom_sort(std::move(custom_sort));
}

void dataframe_interface::add_dimension(
    std::span<const char *const> dimension_categories,
    std::span<const std::uint32_t> dimension_values,
    std::string_view name,
    adding_type adding_strategy,
    std::span<const std::pair<const char *, const char *>> info) &
{
	as_impl(this).add_dimension(dimension_categories,
	    dimension_values,
	    name,
	    adding_strategy,
	    info);
}

void dataframe_interface::add_measure(
    std::span<const double> measure_values,
    std::string_view name,
    adding_type adding_strategy,
    std::span<const std::pair<const char *, const char *>> info) &
{
	as_impl(this).add_measure(measure_values,
	    name,
	    adding_strategy,
	    info);
}

void dataframe_interface::add_series_by_other(
    std::string_view curr_series,
    std::string_view name,
    std::function<cell_value(record_type, cell_value)>
        value_transform,
    std::span<const std::pair<const char *, const char *>> info) &
{
	as_impl(this).add_series_by_other(curr_series,
	    name,
	    std::move(value_transform),
	    info);
}

void dataframe_interface::remove_series(
    std::span<const std::string_view> names) &
{
	as_impl(this).remove_series(names);
}

void dataframe_interface::add_record(
    std::span<const cell_value> values) &
{
	as_impl(this).add_record(values);
}

void dataframe_interface::remove_records(
    std::span<const std::size_t> record_ids) &
{
	as_impl(this).remove_records(record_ids);
}

void dataframe_interface::remove_records(
    std::function<bool(record_type)> filter) &
{
	as_impl(this).remove_records(std::move(filter));
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

bool dataframe_interface::has_na(std::string_view column) const &
{
	return as_impl(this).has_na(column);
}

void dataframe_interface::fill_na(std::string_view column,
    cell_value value) &
{
	as_impl(this).fill_na(column, value);
}

void dataframe_interface::finalize() & { as_impl(this).finalize(); }

std::span<const std::string>
dataframe_interface::get_dimensions() const &
{
	return as_impl(this).get_dimensions();
}

std::span<const std::string>
dataframe_interface::get_measures() const &
{
	return as_impl(this).get_measures();
}

std::span<const std::string> dataframe_interface::get_categories(
    std::string_view dimension) const &
{
	return as_impl(this).get_categories(dimension);
}

cell_value dataframe_interface::get_data(record_identifier record_id,
    std::string_view column) const &
{
	return as_impl(this).get_data(record_id, column);
}

std::size_t dataframe_interface::get_record_count() const &
{
	return as_impl(this).get_record_count();
}

std::string_view dataframe_interface::get_series_info(
    const std::string_view &id,
    const char *key) const &
{
	return as_impl(this).get_series_info(id, key);
}

bool dataframe_interface::is_filtered(std::size_t record_id) const &
{
	return as_impl(this).is_filtered(record_id);
}

std::string dataframe_interface::get_record_id_by_dims(
    std::size_t my_record,
    std::span<const std::string> dimensions) const &
{
	return as_impl(this).get_record_id_by_dims(my_record, dimensions);
}

}