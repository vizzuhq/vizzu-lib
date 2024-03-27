#ifndef VIZZU_DATAFRAME_DATAFRAME_H
#define VIZZU_DATAFRAME_DATAFRAME_H

#include <memory>

#include "../interface.h"

#include "data_source.h"

namespace Vizzu::dataframe
{

class dataframe final : public dataframe_interface
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
	dataframe() noexcept = default;
	dataframe(std::shared_ptr<const data_source> other,
	    std::vector<bool> const *filtered,
	    std::vector<std::size_t> const *sorted);

	std::shared_ptr<dataframe_interface> copy(bool remove_filtered,
	    bool inherit_sorting) const & final;

	std::string set_aggregate(series_identifier series,
	    const any_aggregator_type &aggregator)
	    & final;

	void set_filter(std::function<bool(record_type)> &&filt) & final;

	void set_sort(series_identifier series,
	    any_sort_type sort,
	    na_position na_pos)
	    & final;

	void set_custom_sort(
	    std::function<std::weak_ordering(record_type, record_type)>
	        custom_sort)
	    & final;

	void add_dimension(
	    std::span<const char *const> dimension_categories,
	    std::span<const std::uint32_t> dimension_values,
	    const char *name,
	    adding_type adding_strategy,
	    std::span<const std::pair<const char *, const char *>> info)
	    & final;

	void add_measure(std::span<const double> measure_values,
	    const char *name,
	    adding_type adding_strategy,
	    std::span<const std::pair<const char *, const char *>> info)
	    & final;

	void add_series_by_other(series_identifier curr_series,
	    const char *name,
	    std::function<cell_value(record_type, cell_value)>
	        value_transform,
	    std::span<const std::pair<const char *, const char *>> info)
	    & final;

	void remove_series(std::span<const series_identifier> names)
	    & final;

	void add_record(std::span<const cell_value> values) & final;

	void remove_records(std::span<const record_identifier> record_ids)
	    & final;

	void remove_records(std::function<bool(record_type)> filter)
	    & final;

	void remove_unused_categories(series_identifier column) & final;

	void change_data(record_identifier record_id,
	    series_identifier column,
	    cell_value value)
	    & final;

	bool has_na(series_identifier column) const & final;

	void fill_na(series_identifier column, cell_value value) & final;

	void finalize() & final;

	std::string as_string() const & final;

	std::span<const std::string> get_dimensions() const & final;

	std::span<const std::string> get_measures() const & final;

	std::span<const std::string> get_categories(
	    series_identifier dimension) const & final;

	std::pair<double, double> get_min_max(
	    series_identifier measure) const & final;

	[[nodiscard]] std::string_view get_series_name(
	    const series_identifier &id) const & final;

	[[nodiscard]] std::string_view get_record_unique_id(
	    record_identifier id) const & final;

	[[nodiscard]] std::string_view get_series_info(
	    const series_identifier &id,
	    const char *key) const & final;

	cell_value get_data(record_identifier record_id,
	    series_identifier column) const & final;

	std::size_t get_record_count() const & final
	{
		return get_data_source().get_record_count();
	}

	[[nodiscard]] bool is_filtered(
	    record_identifier record_id) const & final;

private:
	void migrate_data();
	void change_state_to(state_type new_state,
	    state_modification_reason reason);

	const data_source &get_data_source() const;

	void visit(std::function<void(record_type)> function) const;

	void visit(const std::function<void(record_type)> &function,
	    const std::vector<std::size_t> *sort,
	    const std::vector<bool> *filt) const;

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
