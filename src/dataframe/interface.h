#ifndef VIZZU_DATAFRAME_INTERFACE_H
#define VIZZU_DATAFRAME_INTERFACE_H

#include <functional>
#include <memory>
#include <span>
#include <string_view>
#include <variant>

namespace Vizzu::dataframe
{

enum class value_type { dimension, measure };
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
	override_and_add,
	override_all_with_rotation
};

struct custom_aggregator
{
	std::function<void(double)> add;
	std::function<double()> get;
};

class dataframe_interface :
    std::enable_shared_from_this<dataframe_interface>
{
public:
	struct record_type
	{
		std::function<std::pair<value_type, std::string>(
		    const char *)>
		    getValueByColumn;

		std::string recordId;
	};

	virtual ~dataframe_interface() = default;

	virtual std::shared_ptr<dataframe_interface> copy() const & = 0;

	virtual void set_aggregate(std::span<const char *> dimensions,
	    std::span<std::pair<const char *,
	        std::variant<aggregator_type, custom_aggregator>>>
	        measures) & = 0;

	virtual void set_filter(
	    std::function<bool(record_type)> filter) & = 0;

	virtual void set_sort(
	    std::span<std::pair<const char *, sort_type>> columns) & = 0;

	virtual void set_sort(
	    std::function<std::weak_ordering(record_type, record_type)>
	        custom_sort) & = 0;

	virtual void add_dimension(
	    std::span<const char *> dimension_values,
	    const char *name,
	    adding_type adding_strategy =
	        adding_type::create_or_add) & = 0;

	virtual void add_measure(std::span<const double> measure_values,
	    const char *name,
	    const char *unit = "",
	    adding_type adding_strategy =
	        adding_type::create_or_add) & = 0;

	virtual void add_series_by_other(const char *curr_series,
	    const char *new_name,
	    std::function<const char *(record_type, std::string)>
	        value_transform,
	    const char *unit = nullptr) & = 0;

	virtual void remove_series(std::span<const char *> names) & = 0;

	virtual void add_records(
	    std::span<std::span<const char *>> values) & = 0;

	virtual void remove_record(const char *record_id) & = 0;

	virtual void remove_records(
	    std::function<bool(record_type)> filter) & = 0;

	virtual void change_data(const char *record_id,
	    const char *column,
	    const char *value) & = 0;

	virtual void change_data(const char *column,
	    const char *value) & = 0;

	virtual void fill_na(const char *column, double value) & = 0;

	virtual std::string get_data() const & = 0;

	virtual std::span<std::string> get_categories(
	    const char *dimension) const & = 0;

	virtual std::pair<double, double> get_min_max(
	    const char *measure) const & = 0;
};
}

#endif // VIZZU_DATAFRAME_INTERFACE_H
