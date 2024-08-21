#ifndef VIZZU_DATAFRAME_INTERFACE_H
#define VIZZU_DATAFRAME_INTERFACE_H

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <set>
#include <span>
#include <string_view>
#include <variant>

namespace Vizzu::Data
{
struct RowWrapper;
}

namespace Vizzu::dataframe
{

using cell_value = std::variant<double, std::string_view>;
using cell_reference = std::variant<double, const std::string *>;

enum class aggregator_type : std::uint8_t {
	sum,
	min,
	max,
	mean,
	count,
	distinct
};

enum class sort_type : std::uint8_t {
	less,
	greater,
	natural_less,
	natural_greater,
	by_categories
};

enum class na_position : std::uint8_t { last, first };

enum class adding_type : std::uint8_t {
	create_or_add,
	create_or_throw,
	create_or_override,
	override_full,
	override_all_with_rotation
};

struct custom_aggregator
{
	std::string_view name;
	using id_type = std::variant<double,
	    std::pair<double, std::size_t>,
	    std::set<const std::string *>>;
	id_type (*create)();
	double (*add)(id_type &, cell_reference const &);

	auto operator<=>(const custom_aggregator &oth) const
	{
		return name <=> oth.name;
	}

	auto operator==(const custom_aggregator &oth) const
	{
		return name == oth.name;
	}
};

constexpr std::size_t align_impl = alignof(double);
constexpr std::size_t max_size_impl = 26 * sizeof(std::intptr_t);

class alignas(align_impl) dataframe_interface
{
public:
	using record_identifier = std::variant<std::string, std::size_t>;

	using any_aggregator_type = std::optional<aggregator_type>;

	using any_sort_type = sort_type;

	using record_type = Data::RowWrapper;

	[[nodiscard]] std::shared_ptr<dataframe_interface> copy(
	    bool inherit_sorting) const &;

	[[nodiscard]] std::string set_aggregate(
	    const std::string_view &series,
	    const any_aggregator_type &aggregator) &;

	void aggregate_by(const std::string_view &series)
	{
		std::ignore = set_aggregate(series, {});
	}

	void set_sort(const std::string_view &series,
	    any_sort_type sort,
	    na_position na_pos) &;

	void set_custom_sort(
	    const std::function<std::weak_ordering(record_type,
	        record_type)> &custom_sort) &;

	void add_dimension(
	    std::span<const char *const> dimension_categories,
	    std::span<const std::uint32_t> dimension_values,
	    std::string_view name,
	    adding_type adding_strategy,
	    std::span<const std::pair<const char *, const char *>> info)
	    &;

	void add_measure(std::span<const double> measure_values,
	    std::string_view name,
	    adding_type adding_strategy,
	    std::span<const std::pair<const char *, const char *>> info)
	    &;

	void add_series_by_other(std::string_view curr_series,
	    std::string_view name,
	    const std::function<cell_value(record_type, cell_reference)>
	        &value_transform,
	    std::span<const std::pair<const char *, const char *>> info)
	    &;

	void remove_series(std::span<const std::string_view> names) &;

	void add_record(std::span<const char *const> values) &;

	void remove_records(
	    const std::function<bool(const record_type &)> &filter) &;

	void remove_unused_categories(std::string_view column) &;

	void change_data(std::size_t record_id,
	    std::string_view column,
	    cell_value value) &;

	[[nodiscard]] bool has_na(const std::string_view &column) const &;

	void fill_na(std::string_view column, cell_value value) &;

	void finalize() &;

	[[nodiscard]] std::span<const std::string>
	get_dimensions() const &;

	[[nodiscard]] std::span<const std::string> get_measures() const &;

	[[nodiscard]] std::span<const std::string> get_categories(
	    const std::string_view &dimension) const &;

	[[nodiscard]] cell_reference get_data(
	    const record_identifier &record_id,
	    const std::string_view &column) const &;

	[[nodiscard]] std::size_t get_record_count() const &;

	[[nodiscard]] std::string_view get_series_info(
	    const std::string_view &id,
	    const char *key) const &;

	[[nodiscard]] bool is_removed(std::size_t record_id) const &;

	[[nodiscard]] std::string get_record_id_by_dims(
	    std::size_t my_record,
	    std::span<const std::string> dimensions) const &;

	// NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
	alignas(align_impl) std::byte data[max_size_impl];
};

#ifdef __cpp_lib_is_pointer_interconvertible
static_assert(std::is_pointer_interconvertible_with_class(
    &dataframe_interface::data));
#endif
}

#endif // VIZZU_DATAFRAME_INTERFACE_H
