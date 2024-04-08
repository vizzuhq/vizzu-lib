#ifndef VIZZU_DATAFRAME_DATAFRAME_H
#define VIZZU_DATAFRAME_DATAFRAME_H

#include <memory>

#include "../interface.h"

#include "data_source.h"

namespace Vizzu::dataframe
{

class dataframe
{
	enum class state_type {
		modifying,
		aggregating,
		sorting,
		finalized
	};

	enum class source_type { owning, copying };

	enum class state_modification_reason {
		needs_series_type,
		needs_record_count,
		needs_sorted_records,
		needs_own_state
	};

	struct copy_source
	{
		std::shared_ptr<const data_source> other;
		std::optional<std::vector<std::size_t>> sorted_indices;
		std::optional<std::vector<bool>> pre_remove;
	};

public:
	using record_identifier = dataframe_interface::record_identifier;
	using record_type = dataframe_interface::record_type;
	using any_aggregator_type =
	    dataframe_interface::any_aggregator_type;
	using any_sort_type = dataframe_interface::any_sort_type;

	dataframe() noexcept = default;
	dataframe(std::shared_ptr<const data_source> other,
	    std::vector<bool> const *filtered,
	    std::vector<std::size_t> const *sorted);

	[[nodiscard]] std::shared_ptr<dataframe_interface>
	copy(bool remove_filtered, bool inherit_sorting) const &;

	[[nodiscard]] static std::shared_ptr<dataframe_interface>
	create_new();

	[[nodiscard]] std::string set_aggregate(std::string_view series,
	    const any_aggregator_type &aggregator) &;

	void set_filter(std::function<bool(record_type)> &&filt) &;

	void set_sort(std::string_view series,
	    any_sort_type sort,
	    na_position na_pos) &;

	void set_custom_sort(
	    std::function<std::weak_ordering(record_type, record_type)>
	        custom_sort) &;

	void add_dimension(
	    std::span<const char *const> dimension_categories,
	    std::span<const std::uint32_t> dimension_values,
	    const char *name,
	    adding_type adding_strategy,
	    std::span<const std::pair<const char *, const char *>> info)
	    &;

	void add_measure(std::span<const double> measure_values,
	    const char *name,
	    adding_type adding_strategy,
	    std::span<const std::pair<const char *, const char *>> info)
	    &;

	void add_series_by_other(std::string_view curr_series,
	    const char *name,
	    std::function<cell_value(record_type, cell_value)>
	        value_transform,
	    std::span<const std::pair<const char *, const char *>> info)
	    &;

	void remove_series(std::span<const std::string_view> names) &;

	void add_record(std::span<const cell_value> values) &;

	void remove_records(std::span<const std::size_t> record_ids) &;

	void remove_records(std::function<bool(record_type)> filter) &;

	void remove_unused_categories(std::string_view column) &;

	void change_data(std::size_t record_id,
	    std::string_view column,
	    cell_value value) &;

	[[nodiscard]] bool has_na(std::string_view column) const &;

	void fill_na(std::string_view column, cell_value value) &;

	void finalize() &;

	[[nodiscard]] std::string as_string() const &;

	[[nodiscard]] std::span<const std::string>
	get_dimensions() const &;

	[[nodiscard]] std::span<const std::string> get_measures() const &;

	[[nodiscard]] std::span<const std::string> get_categories(
	    std::string_view dimension) const &;

	[[nodiscard]] std::pair<double, double> get_min_max(
	    std::string_view measure) const &;

	[[nodiscard]] std::string_view get_series_name(
	    const std::string_view &id) const &;

	[[nodiscard]] std::string_view get_series_info(
	    const std::string_view &id,
	    const char *key) const &;

	[[nodiscard]] cell_value get_data(record_identifier record_id,
	    std::string_view column) const &;

	[[nodiscard]] std::size_t get_record_count() const &
	{
		return get_data_source().get_record_count();
	}

	[[nodiscard]] bool is_filtered(
	    record_identifier record_id) const &;

	[[nodiscard]] std::string get_record_id_by_dims(
	    std::size_t my_record,
	    std::span<const std::string> dimensions) const &;

	[[nodiscard]] std::size_t get_series_orig_index(
	    std::string_view series) const;

	[[nodiscard]] series_type get_series_type(
	    std::string_view series) const;

private:
	void migrate_data();
	void change_state_to(state_type new_state,
	    state_modification_reason reason);

	[[nodiscard]] const data_source &get_data_source() const;

	void visit(
	    const std::function<void(record_type)> &function) const;

	void visit(const std::function<void(record_type)> &function,
	    const std::vector<std::size_t> *sort,
	    const std::vector<bool> *filt) const;

	[[nodiscard]] const dataframe_interface *as_if() const
	{
		return std::launder(static_cast<const dataframe_interface *>(
		    static_cast<const void *>(this)));
	}

	Refl::EnumVariant<source_type,
	    std::shared_ptr<data_source>,
	    copy_source>
	    source{std::make_shared<data_source>()};

	Refl::EnumVariant<state_type,
	    std::vector<std::string>,
	    data_source::aggregating_type,
	    data_source::sorting_type,
	    std::reference_wrapper<const data_source::final_info>>
	    state_data;

	std::variant<std::function<bool(record_type)>, std::vector<bool>>
	    filter;
};

}

#endif // VIZZU_DATAFRAME_DATAFRAME_H
