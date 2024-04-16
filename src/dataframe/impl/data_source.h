#ifndef VIZZU_DATAFRAME_DATA_SOURCE_H
#define VIZZU_DATAFRAME_DATA_SOURCE_H

#include <cmath>
#include <limits>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "../interface.h"
#include "base/refl/auto_enum.h"

namespace Vizzu::dataframe
{

enum class series_type { unknown, dimension, measure };

[[maybe_unused]] [[noreturn]] void error();

class data_source : public std::enable_shared_from_this<data_source>
{
public:
	using record_type = dataframe_interface::record_type;

private:
	constexpr static std::uint32_t nav = ~std::uint32_t{};
	constexpr static double nan =
	    std::numeric_limits<double>::quiet_NaN();

	constexpr static auto is_nav = [](const std::uint32_t &v)
	{
		return v == nav;
	};

	constexpr static auto is_nan =
	    [](const double &d) noexcept -> bool
	{
		return std::isnan(d);
	};

	struct dimension_t
	{
		std::vector<std::string> categories;
		na_position na_pos{na_position::last};
		std::vector<std::uint32_t> values;
		std::map<std::string, std::string> info;
		bool contains_nav{};

		dimension_t() noexcept = default;

		template <class Range1, class Range2, class Range3>
		dimension_t(Range1 &&categories,
		    Range2 &&values,
		    Range3 &&info) :
		    categories(std::begin(categories), std::end(categories)),
		    values(std::begin(values), std::end(values)),
		    info(std::begin(info), std::end(info)),
		    contains_nav{std::any_of(this->values.begin(),
		        this->values.end(),
		        is_nav)}
		{}

		void add_more_data(std::span<const char *const> categories,
		    std::span<const std::uint32_t> values);

		[[nodiscard]] std::vector<std::size_t> get_indices(
		    const dataframe_interface::any_sort_type &sorter) const;

		void sort_by(std::vector<std::size_t> &&indices,
		    na_position na_pos);

		std::uint32_t get_or_set_cat(std::string_view cat);

		void add_element(std::string_view const &cat);

		[[nodiscard]] std::string_view get(std::size_t index) const;

		void set(std::size_t index, std::string_view value);

		void set_nav(std::string_view value, std::size_t to_size);

		[[nodiscard]] std::vector<bool> get_categories_usage() const;

		void remove_unused_categories(std::vector<bool> &&used);
	};

	struct measure_t
	{
		std::vector<double> values;
		std::map<std::string, std::string> info;
		bool contains_nan{};

		measure_t() noexcept = default;

		template <class Range1, class Range2>
		measure_t(Range1 &&values, Range2 &&info) :
		    values(std::begin(values), std::end(values)),
		    info(std::begin(info), std::end(info)),
		    contains_nan(std::any_of(this->values.begin(),
		        this->values.end(),
		        is_nan))
		{}

		[[nodiscard]] const double &get(std::size_t index) const;

		[[nodiscard]] std::pair<double, double> get_min_max() const;
	};

	using final_info = std::map<std::string, std::size_t>;

	std::string get_id(std::size_t record,
	    std::span<const std::string> series) const;

	using series_data = Refl::EnumVariant<series_type,
	    std::monostate,
	    std::pair<std::string_view, dimension_t &>,
	    std::pair<std::string_view, measure_t &>>;

	using const_series_data = Refl::EnumVariant<series_type,
	    std::monostate,
	    std::pair<std::string_view, const dimension_t &>,
	    std::pair<std::string_view, const measure_t &>>;

	// replace these to std::flat_map
	std::vector<std::string> measure_names; // sorted by name
	std::vector<measure_t> measures;

	std::vector<std::string> dimension_names; // sorted by name
	std::vector<dimension_t> dimensions;

	final_info finalized;

	struct sorter;

public:
	struct sort_one_series
	{
		const_series_data data;
		sort_type sort_type{};
		na_position na_pos{};
	};

	using sorting_type = std::vector<sort_one_series>;

	struct aggregating_type
	{
		std::map<std::string_view,
		    std::reference_wrapper<const dimension_t>>
		    dims;
		std::map<std::string,
		    std::pair<const_series_data, custom_aggregator>,
		    std::less<>>
		    meas;

		std::pair<std::string, bool> add_aggregated(
		    const_series_data &&data,
		    const aggregator_type &aggregator);
	};

	data_source() = default;

	data_source(const std::shared_ptr<const data_source> &copying,
	    std::optional<std::vector<bool>> &&filtered,
	    std::optional<std::vector<std::size_t>> &&sorted);

	data_source(aggregating_type &&aggregating,
	    std::vector<bool> const *filtered,
	    std::size_t record_count);

	[[nodiscard]] std::size_t get_record_count() const;

	[[nodiscard]] std::vector<std::size_t> get_sorted_indices(
	    const sorting_type &sorters) const;

	void sort(std::vector<std::size_t> &&indices);

	std::size_t change_series_identifier_type(
	    const std::string_view &name) const;

	std::size_t change_record_identifier_type(
	    const std::string &id) const;

	void normalize_sizes();

	void remove_series(std::span<const std::size_t> dims,
	    std::span<const std::size_t> meas);

	void remove_records(std::span<const std::size_t> indices);

	[[nodiscard]] cell_value get_data(std::size_t record_id,
	    const std::string_view &column) const;

	[[nodiscard]] series_data get_series(const std::string_view &id);
	[[nodiscard]] const_series_data get_series(
	    const std::string_view &id) const;

	[[nodiscard]] std::pair<double, double> get_min_max(
	    const measure_t &measure) const;

	void finalize();

	dimension_t &add_new_dimension(
	    std::span<const char *const> dimension_categories,
	    std::span<const std::uint32_t> dimension_values,
	    std::string_view name,
	    std::span<const std::pair<const char *, const char *>> info);

	dimension_t &add_new_dimension(dimension_t &&dim,
	    std::string_view name);

	measure_t &add_new_measure(std::span<const double> measure_values,
	    std::string_view name,
	    std::span<const std::pair<const char *, const char *>> info);

	measure_t &add_new_measure(measure_t &&measure,
	    std::string_view name);

	static std::vector<std::size_t> get_sorted_indices(
	    std::size_t max,
	    bool (*sort)(std::size_t, std::size_t));

	friend class dataframe;
};

}

#endif // VIZZU_DATAFRAME_DATA_SOURCE_H
