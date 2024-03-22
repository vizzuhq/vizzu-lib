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

class data_source : public std::enable_shared_from_this<data_source>
{
public:
	using record_type = dataframe_interface::record_type;
	using record_identifier = dataframe_interface::record_identifier;
	using series_identifier = dataframe_interface::series_identifier;

private:
	constexpr static std::uint32_t nav = ~std::uint32_t{};
	constexpr static double nan =
	    std::numeric_limits<double>::quiet_NaN();

	struct dimension_t
	{
		std::vector<std::string> categories;
		na_position na_pos{na_position::last};
		std::vector<std::uint32_t> values;
		std::map<std::string, std::string> info;
		bool contains_nav;

		dimension_t() noexcept = default;

		template <class Range1, class Range2, class Range3>
		dimension_t(Range1 &&categories,
		    Range2 &&values,
		    Range3 &&info) :
		    categories(std::begin(categories), std::end(categories)),
		    values(std::begin(values), std::end(values)),
		    info(std::begin(info), std::end(info)),
		    contains_nav{std::ranges::any_of(this->values,
		        std::bind_front(std::equal_to{}, nav))}
		{}

		void add_more_data(std::span<const char *const> categories,
		    std::span<const std::uint32_t> values);

		[[nodiscard]] std::vector<std::uint32_t> get_indices(
		    const dataframe_interface::any_sort_type &sorter) const;

		void sort_by(std::vector<std::uint32_t> &&indices,
		    na_position na_pos);

		std::uint32_t get_or_set_cat(std::string_view cat);

		void add_element(std::string_view const &cat);

		[[nodiscard]] std::string_view get(std::size_t index) const;

		void set(std::size_t index, std::string_view value);

		void set_nav(std::string_view value);
	};

	struct measure_t
	{
		std::vector<double> values;
		std::map<std::string, std::string> info;
		bool contains_nan;

		measure_t() noexcept = default;

		template <class Range1, class Range2>
		measure_t(Range1 &&values, Range2 &&info) :
		    values(std::begin(values), std::end(values)),
		    info(std::begin(info), std::end(info)),
		    contains_nan(std::ranges::any_of(this->values,
		        static_cast<bool (&)(double)>(std::isnan)))
		{}

		const double &get(std::size_t index) const;

		[[nodiscard]] std::pair<double, double> get_min_max() const;
	};

	struct final_info
	{
		std::unordered_map<std::string_view, std::size_t>
		    series_to_index;
		std::vector<std::pair<double, double>> min_max;
		std::vector<std::string> record_unique_ids;
		std::unordered_map<std::string_view, std::size_t>
		    record_to_index;

		explicit final_info(const data_source &source);
	};

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

	std::optional<final_info> finalized;

	struct sorter;

public:
	using sort_one_series =
	    std::tuple<const_series_data, sort_type, na_position>;

	using sorting_type = std::vector<std::variant<sort_one_series,
	    std::function<std::weak_ordering(record_type, record_type)>>>;

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
		    const custom_aggregator &aggregator);
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
	    const dataframe_interface *parent,
	    const sorting_type &sorters) const;

	void sort(std::vector<std::size_t> &&indices);

	void change_series_identifier_type(series_identifier &id) const;

	void change_record_identifier_type(record_identifier &id) const;

	void normalize_sizes();

	void remove_series(std::span<const std::size_t> dims,
	    std::span<const std::size_t> meas);

	void remove_records(std::span<const std::size_t> indices);

	[[nodiscard]] cell_value get_data(std::size_t record_id,
	    const series_identifier &column) const;

	[[nodiscard]] series_data get_series(series_identifier id);
	[[nodiscard]] const_series_data get_series(
	    series_identifier id) const;

	[[nodiscard]] std::pair<double, double> get_min_max(
	    const measure_t &measure) const;

	[[nodiscard]] const final_info &finalize()
	{
		normalize_sizes();
		return finalized ? *finalized : finalized.emplace(*this);
	}

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

	friend class dataframe;
};

}

#endif // VIZZU_DATAFRAME_DATA_SOURCE_H
