#ifndef VIZZU_DATAFRAME_INTERFACE_H
#define VIZZU_DATAFRAME_INTERFACE_H

#include <any>
#include <functional>
#include <memory>
#include <span>
#include <string_view>
#include <variant>

namespace Vizzu::dataframe
{

using cell_value = std::variant<double, std::string_view>;

enum class aggregator_type {
	sum,
	min,
	max,
	mean,
	count,
	distinct,
	exists
};

enum class sort_type { no, less, greater };

enum class adding_type {
	create_or_add,
	create_or_throw,
	override_full,
	override_all_with_rotation
};

struct custom_aggregator
{
	using id_type = std::any;
	std::function<std::pair<id_type, std::string>(std::string_view)>
	    create;
	std::function<double(const id_type &, double)> add;
};

class dataframe_interface :
    std::enable_shared_from_this<dataframe_interface>
{
public:
	using series_identifier = std::variant<const char *, std::size_t>;
	using record_identifier = std::variant<const char *, std::size_t>;

	struct record_type
	{
		std::function<cell_value(series_identifier)> getValueByColumn;

		record_identifier recordId;
	};

	virtual ~dataframe_interface() = default;

	virtual std::shared_ptr<dataframe_interface> copy() const & = 0;

	virtual void set_aggregate(series_identifier series,
	    std::variant<std::monostate,
	        aggregator_type,
	        custom_aggregator> aggregator = {}) & = 0;

	virtual void set_filter(
	    std::function<bool(record_type)> filter) & = 0;

	virtual void set_sort(series_identifier series,
	    sort_type sort) & = 0;

	virtual void set_sort(
	    std::function<std::weak_ordering(record_type, record_type)>
	        custom_sort) & = 0;

	virtual void add_dimension(
	    std::span<const char *> dimension_categories,
	    std::span<std::size_t> dimension_indices,
	    const char *name,
	    adding_type adding_strategy = adding_type::create_or_add,
	    std::span<std::pair<const char *, const char *>> info =
	        {}) & = 0;

	virtual void add_measure(std::span<const double> measure_values,
	    const char *name,
	    adding_type adding_strategy = adding_type::create_or_add,
	    std::span<std::pair<const char *, const char *>> info =
	        {}) & = 0;

	virtual void add_series_by_other(series_identifier curr_series,
	    const char *name,
	    std::function<cell_value(record_type, cell_value)>
	        value_transform,
	    std::span<std::pair<const char *, const char *>> info =
	        {}) & = 0;

	virtual void remove_series(
	    std::span<series_identifier> names) & = 0;

	virtual void add_records(std::span<cell_value> values) & = 0;

	virtual void remove_records(
	    std::span<record_identifier> record_ids) & = 0;

	virtual void remove_records(
	    std::function<bool(record_type)> filter) & = 0;

	virtual void change_data(record_identifier record_id,
	    series_identifier column,
	    cell_value value) & = 0;

	virtual void fill_na(series_identifier column,
	    double value) & = 0;

	virtual std::string as_string() const & = 0;

	virtual std::span<std::string> get_dimensions() const & = 0;
	virtual std::span<std::string> get_measures() const & = 0;

	virtual std::span<std::string> get_categories(
	    series_identifier dimension) const & = 0;

	virtual std::pair<double, double> get_min_max(
	    series_identifier measure) const & = 0;

	virtual series_identifier change_series_identifier_type(
	    const series_identifier &id) const & = 0;

	virtual record_identifier change_record_identifier_type(
	    const record_identifier &id) const & = 0;

	virtual cell_value get_data(record_identifier record_id,
	    series_identifier column) const & = 0;

	virtual void visit(std::function<void(record_type)>) const & = 0;
};
}

#endif // VIZZU_DATAFRAME_INTERFACE_H
