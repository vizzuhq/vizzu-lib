
#include "aggregators.h"

#include <cmath>
#include <cstddef>
#include <limits>
#include <set>
#include <string>
#include <utility>
#include <variant>

#include "base/refl/auto_enum.h"
#include "dataframe/interface.h"

namespace Vizzu::dataframe
{

inline bool is_valid(cell_reference const &value)
{
	const double *d = std::get_if<double>(&value);
	return d ? !std::isnan(*d)
	         : *std::get_if<const std::string *>(&value) != nullptr;
}

Refl::EnumArray<aggregator_type, custom_aggregator>
get_aggregators() noexcept
{
	constinit static const auto &&empty_double =
	    []() -> custom_aggregator::id_type
	{
		return double{};
	};
	constinit static const auto &&init_nan =
	    []() -> custom_aggregator::id_type
	{
		return std::numeric_limits<double>::quiet_NaN();
	};

	auto &&aggrs = Refl::enum_names<aggregator_type>;
	return {{{{aggrs[static_cast<std::size_t>(aggregator_type::sum)],
	              empty_double,
	              [](custom_aggregator::id_type &id,
	                  cell_reference const &cell) -> double
	              {
		              auto &ref = *std::get_if<double>(&id);
		              const double &value =
		                  *std::get_if<double>(&cell);
		              if (std::isfinite(value)) ref += value;

		              return ref;
	              }},
	    {aggrs[static_cast<std::size_t>(aggregator_type::min)],
	        init_nan,
	        [](custom_aggregator::id_type &id,
	            cell_reference const &cell) -> double
	        {
		        auto &ref = *std::get_if<double>(&id);
		        const auto &value = *std::get_if<double>(&cell);
		        if (std::isfinite(value) && !(value >= ref))
			        ref = value;

		        return ref;
	        }},
	    {aggrs[static_cast<std::size_t>(aggregator_type::max)],
	        init_nan,
	        [](custom_aggregator::id_type &id,
	            cell_reference const &cell) -> double
	        {
		        auto &ref = *std::get_if<double>(&id);
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
	            cell_reference const &cell) -> double
	        {
		        auto &[sum, count] =
		            *std::get_if<std::pair<double, std::size_t>>(&id);
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
	            cell_reference const &cell) -> double
	        {
		        auto &s = *std::get_if<double>(&id);
		        if (is_valid(cell)) s += 1;
		        return s;
	        }},
	    {aggrs[static_cast<std::size_t>(aggregator_type::distinct)],
	        []() -> custom_aggregator::id_type
	        {
		        return std::set<const std::string *>{};
	        },
	        [](custom_aggregator::id_type &id,
	            cell_reference const &cell) -> double
	        {
		        auto &set =
		            *std::get_if<std::set<const std::string *>>(&id);
		        if (const std::string *v =
		                *std::get_if<const std::string *>(&cell))
			        set.insert(v);
		        return static_cast<double>(set.size());
	        }}}}};
}

const Refl::EnumArray<aggregator_type, custom_aggregator>
    aggregators = get_aggregators();

}