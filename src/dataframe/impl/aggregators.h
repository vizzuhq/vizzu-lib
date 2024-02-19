#ifndef VIZZU_DATAFRAME_AGGREGATORS_H
#define VIZZU_DATAFRAME_AGGREGATORS_H

#include <cmath>
#include <limits>
#include <set>

#include "../interface.h"
#include "base/refl/auto_enum.h"

namespace Vizzu::dataframe
{

// NOLINTNEXTLINE(fuchsia-statically-constructed-objects)
constinit const static inline Refl::EnumArray<aggregator_type,
    custom_aggregator>
    aggregators{{{{std::string_view{"sum"},
                      []() -> custom_aggregator::id_type
                      {
	                      return 0.0;
                      },
                      [](custom_aggregator::id_type &id,
                          double value) -> double
                      {
	                      auto &ref = *std::any_cast<double>(&id);
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
            [](custom_aggregator::id_type &id, double value) -> double
            {
	            auto &ref = *std::any_cast<double>(&id);
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
            [](custom_aggregator::id_type &id, double value) -> double
            {
	            auto &ref = *std::any_cast<double>(&id);
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
            [](custom_aggregator::id_type &id, double value) -> double
            {
	            auto &[sum, count] =
	                *std::any_cast<std::pair<double, std::size_t>>(
	                    &id);
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
            [](custom_aggregator::id_type &id, double value) -> double
            {
	            auto &s = *std::any_cast<std::size_t>(&id);
	            if (!std::isnan(value) && !std::isinf(value)) s += 1;
	            return static_cast<double>(s);
            }},
        {std::string_view{"distinct"},
            []() -> custom_aggregator::id_type
            {
	            return std::set<double>{};
            },
            [](custom_aggregator::id_type &id, double value) -> double
            {
	            auto &set = *std::any_cast<std::set<double>>(&id);
	            if (!std::isnan(value) && !std::isinf(value))
		            set.insert(value);
	            return static_cast<double>(set.size());
            }},
        {std::string_view{"exists"},
            []() -> custom_aggregator::id_type
            {
	            return bool{};
            },
            [](custom_aggregator::id_type &id, double value) -> double
            {
	            auto &b = *std::any_cast<bool>(&id);
	            if (!std::isnan(value) && !std::isinf(value))
		            b = true;
	            return b;
            }}}}};
}

#endif // VIZZU_DATAFRAME_AGGREGATORS_H
