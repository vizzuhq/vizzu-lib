
#include "aggregators.h"

#include <cmath>
#include <limits>
#include <set>

namespace Vizzu::dataframe
{

inline bool is_valid(cell_value const &value)
{
	const double *d = std::get_if<double>(&value);
	return d ? !std::isnan(*d)
	         : std::get_if<std::string_view>(&value)->data()
	               != nullptr;
}

Refl::EnumArray<aggregator_type, custom_aggregator>
get_aggregators() noexcept
{
	constinit static auto &&empty_double =
	    []() -> custom_aggregator::id_type
	{
		return double{};
	};
	constinit static auto &&init_nan =
	    []() -> custom_aggregator::id_type
	{
		return std::numeric_limits<double>::quiet_NaN();
	};

	auto &&aggrs = Refl::enum_names<aggregator_type>;
	return {{{{aggrs[static_cast<std::size_t>(aggregator_type::sum)],
	              empty_double,
	              [](custom_aggregator::id_type &id,
	                  cell_value const &cell) -> double
	              {
		              auto &ref = *std::any_cast<double>(&id);
		              const double &value =
		                  *std::get_if<double>(&cell);
		              if (std::isfinite(value)) ref += value;

		              return ref;
	              }},
	    {aggrs[static_cast<std::size_t>(aggregator_type::min)],
	        init_nan,
	        [](custom_aggregator::id_type &id,
	            cell_value const &cell) -> double
	        {
		        auto &ref = *std::any_cast<double>(&id);
		        const auto &value = *std::get_if<double>(&cell);
		        if (std::isfinite(value) && !(value >= ref))
			        ref = value;

		        return ref;
	        }},
	    {aggrs[static_cast<std::size_t>(aggregator_type::max)],
	        init_nan,
	        [](custom_aggregator::id_type &id,
	            cell_value const &cell) -> double
	        {
		        auto &ref = *std::any_cast<double>(&id);
		        const auto &value = *std::get_if<double>(&cell);
		        if (std::isfinite(value) && !(value <= ref))
			        ref = value;

		        return ref;
	        }},
	    {aggrs[static_cast<std::size_t>(aggregator_type::mean)],
	        []() -> custom_aggregator::id_type
	        {
		        return std::pair<double, std::size_t>{};
	        },
	        [](custom_aggregator::id_type &id,
	            cell_value const &cell) -> double
	        {
		        auto &[sum, count] =
		            *std::any_cast<std::pair<double, std::size_t>>(
		                &id);
		        const auto &value = *std::get_if<double>(&cell);
		        if (std::isfinite(value)) {
			        sum += value;
			        ++count;
		        }
		        return count == 0 ? NAN
		                          : sum / static_cast<double>(count);
	        }},
	    {aggrs[static_cast<std::size_t>(aggregator_type::count)],
	        empty_double,
	        [](custom_aggregator::id_type &id,
	            cell_value const &cell) -> double
	        {
		        auto &s = *std::any_cast<double>(&id);
		        if (is_valid(cell)) s += 1;
		        return s;
	        }},
	    {aggrs[static_cast<std::size_t>(aggregator_type::distinct)],
	        []() -> custom_aggregator::id_type
	        {
		        return std::set<const char *>{};
	        },
	        [](custom_aggregator::id_type &id,
	            cell_value const &cell) -> double
	        {
		        auto &set =
		            *std::any_cast<std::set<const char *>>(&id);
		        if (const char *v =
		                std::get_if<std::string_view>(&cell)->data())
			        set.insert(v);
		        return static_cast<double>(set.size());
	        }},
	    {aggrs[static_cast<std::size_t>(aggregator_type::exists)],
	        empty_double,
	        [](custom_aggregator::id_type &id,
	            cell_value const &cell) -> double
	        {
		        auto &b = *std::any_cast<double>(&id);
		        if (is_valid(cell)) b = 1.0;
		        return b;
	        }}}}};
}

const Refl::EnumArray<aggregator_type, custom_aggregator>
    aggregators = get_aggregators();

}