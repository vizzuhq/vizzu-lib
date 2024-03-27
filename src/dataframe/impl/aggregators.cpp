
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

// NOLINTNEXTLINE(fuchsia-statically-constructed-objects)
constinit const Refl::EnumArray<aggregator_type, custom_aggregator>
    aggregators{{{{std::string_view{"sum"},
                      []() -> custom_aggregator::id_type
                      {
	                      return 0.0;
                      },
                      [](custom_aggregator::id_type &id,
                          cell_value const &cell) -> double
                      {
	                      auto &ref = *std::any_cast<double>(&id);
	                      const double &value =
	                          *std::get_if<double>(&cell);
	                      if (!std::isnan(value)
	                          && !std::isinf(value))
		                      ref += value;

	                      return ref;
                      }},
        {std::string_view{"min"},
            []() -> custom_aggregator::id_type
            {
	            return std::numeric_limits<double>::quiet_NaN();
            },
            [](custom_aggregator::id_type &id,
                cell_value const &cell) -> double
            {
	            auto &ref = *std::any_cast<double>(&id);
	            const auto &value = *std::get_if<double>(&cell);
	            if (!std::isnan(value) && !std::isinf(value)
	                && !(value >= ref))
		            ref = value;

	            return ref;
            }},
        {std::string_view{"max"},
            []() -> custom_aggregator::id_type
            {
	            return std::numeric_limits<double>::quiet_NaN();
            },
            [](custom_aggregator::id_type &id,
                cell_value const &cell) -> double
            {
	            auto &ref = *std::any_cast<double>(&id);
	            const auto &value = *std::get_if<double>(&cell);
	            if (!std::isnan(value) && !std::isinf(value)
	                && !(value <= ref))
		            ref = value;

	            return ref;
            }},
        {std::string_view{"mean"},
            []() -> custom_aggregator::id_type
            {
	            return std::pair<double, std::size_t>(0.0, 0);
            },
            [](custom_aggregator::id_type &id,
                cell_value const &cell) -> double
            {
	            auto &[sum, count] =
	                *std::any_cast<std::pair<double, std::size_t>>(
	                    &id);
	            const auto &value = *std::get_if<double>(&cell);
	            if (!std::isnan(value) && !std::isinf(value)) {
		            sum += value;
		            ++count;
	            }
	            return count == 0 ? NAN
	                              : sum / static_cast<double>(count);
            }},
        {std::string_view{"count"},
            []() -> custom_aggregator::id_type
            {
	            return std::size_t{};
            },
            [](custom_aggregator::id_type &id,
                cell_value const &cell) -> double
            {
	            auto &s = *std::any_cast<std::size_t>(&id);
	            if (is_valid(cell)) s += 1;
	            return static_cast<double>(s);
            }},
        {std::string_view{"distinct"},
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
        {std::string_view{"exists"},
            []() -> custom_aggregator::id_type
            {
	            return bool{};
            },
            [](custom_aggregator::id_type &id,
                cell_value const &cell) -> double
            {
	            auto &b = *std::any_cast<bool>(&id);
	            if (is_valid(cell)) b = true;
	            return static_cast<double>(b);
            }}}}};

}