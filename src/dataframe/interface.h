#ifndef VIZZU_DATAFRAME_INTERFACE_H
#define VIZZU_DATAFRAME_INTERFACE_H

#include <any>
#include <functional>
#include <memory>
#include <ranges>
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

enum class sort_type {
	less,
	greater,
	natural_less,
	natural_greater,
	by_categories
};

enum class na_position { last, first };

enum class adding_type {
	create_or_add,
	create_or_throw,
	create_or_override,
	override_full,
	override_all_with_rotation
};

struct custom_aggregator
{
	std::variant<std::string_view, std::string> name;
	using id_type = std::any;
	id_type (*create)();
	double (*add)(id_type &, cell_value const &);

	[[nodiscard]] std::string_view get_name() const
	{
		return std::visit<std::string_view>(std::identity{}, name);
	}

	auto operator<=>(const custom_aggregator &oth) const
	{
		return get_name() <=> oth.get_name();
	}

	auto operator!=(const custom_aggregator &oth) const
	{
		return get_name() != oth.get_name();
	}

	auto operator==(const custom_aggregator &oth) const
	{
		return get_name() == oth.get_name();
	}
};

class dataframe_interface :
    public std::enable_shared_from_this<dataframe_interface>
{
public:
	using series_identifier =
	    std::variant<std::string_view, std::size_t>;
	using record_identifier =
	    std::variant<std::string_view, std::size_t>;

	using any_aggregator_type = std::
	    variant<std::monostate, aggregator_type, custom_aggregator>;

	using any_sort_type = std::variant<sort_type,
	    std::function<std::weak_ordering(std::string_view,
	        std::string_view)>>;

	struct record_type
	{
		[[nodiscard]] cell_value getValue(series_identifier i) const
		{
			return parent->get_data(recordId, i);
		}

		const dataframe_interface *parent;
		record_identifier recordId;

		auto get_dimensions() const
		{
			return std::ranges::transform_view{
			    parent->get_dimensions(),
			    [rec = *this](std::string_view dim)
			        -> std::pair<std::string_view, std::string_view>
			    {
				    auto &&cell = rec.getValue(dim);
				    return {dim,
				        *std::get_if<std::string_view>(&cell)};
			    }};
		}
	};

	virtual ~dataframe_interface() = default;

	[[nodiscard]] virtual std::shared_ptr<dataframe_interface>
	copy(bool remove_filtered, bool inherit_sorting) const & = 0;

	[[nodiscard]] virtual std::string set_aggregate(
	    series_identifier series,
	    const any_aggregator_type &aggregator) & = 0;

	void aggregate_by(series_identifier series)
	{
		[[maybe_unused]] auto &&_ = set_aggregate(series, {});
	}

	virtual void set_filter(
	    std::function<bool(record_type)> &&filter) & = 0;

	virtual void set_sort(series_identifier series,
	    any_sort_type sort,
	    na_position na_pos) & = 0;

	virtual void set_custom_sort(
	    std::function<std::weak_ordering(record_type, record_type)>
	        custom_sort) & = 0;

	virtual void add_dimension(
	    std::span<const char *const> dimension_categories,
	    std::span<const std::uint32_t> dimension_values,
	    const char *name,
	    adding_type adding_strategy,
	    std::span<const std::pair<const char *, const char *>> info)
	    & = 0;

	virtual void add_measure(std::span<const double> measure_values,
	    const char *name,
	    adding_type adding_strategy,
	    std::span<const std::pair<const char *, const char *>> info)
	    & = 0;

	virtual void add_series_by_other(series_identifier curr_series,
	    const char *name,
	    std::function<cell_value(record_type, cell_value)>
	        value_transform,
	    std::span<const std::pair<const char *, const char *>> info)
	    & = 0;

	virtual void remove_series(
	    std::span<const series_identifier> names) & = 0;

	virtual void add_record(std::span<const cell_value> values) & = 0;

	virtual void remove_records(
	    std::span<const record_identifier> record_ids) & = 0;

	virtual void remove_records(
	    std::function<bool(record_type)> filter) & = 0;

	virtual void change_data(record_identifier record_id,
	    series_identifier column,
	    cell_value value) & = 0;

	virtual void fill_na(series_identifier column,
	    cell_value value) & = 0;

	virtual void finalize() & = 0;

	[[nodiscard]] virtual std::string as_string() const & = 0;

	[[nodiscard]] virtual std::span<const std::string>
	get_dimensions() const & = 0;

	[[nodiscard]] virtual std::span<const std::string>
	get_measures() const & = 0;

	[[nodiscard]] virtual std::span<const std::string> get_categories(
	    series_identifier dimension) const & = 0;

	[[nodiscard]] virtual std::pair<double, double> get_min_max(
	    series_identifier measure) const & = 0;

	[[nodiscard]] virtual std::string_view get_series_name(
	    const series_identifier &id) const & = 0;

	[[nodiscard]] virtual std::string_view get_record_unique_id(
	    const record_identifier &id) const & = 0;

	[[nodiscard]] virtual cell_value get_data(
	    record_identifier record_id,
	    series_identifier column) const & = 0;

	[[nodiscard]] virtual std::size_t get_record_count() const & = 0;

	[[nodiscard]] virtual std::string_view get_series_info(
	    const series_identifier &id,
	    const char *key) const & = 0;

	virtual void visit(std::function<void(record_type)> function,
	    bool filtered) const & = 0;
};
}

#endif // VIZZU_DATAFRAME_INTERFACE_H
