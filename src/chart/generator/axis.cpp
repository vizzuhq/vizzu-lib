#include "axis.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>

#include "base/anim/interpolated.h"
#include "base/geom/point.h"
#include "base/math/floating.h"
#include "base/math/interpolation.h"
#include "base/math/range.h"
#include "base/math/renard.h"
#include "chart/options/channel.h"
#include "dataframe/old/types.h"

namespace Vizzu::Gen
{

Geom::Point Axises::origo() const
{
	return {at(AxisId::x).measure.origo(),
	    at(AxisId::y).measure.origo()};
}

MeasureAxis::MeasureAxis(const Math::Range<double> &interval,
    const std::string_view &unit,
    const std::optional<double> &step) :
    enabled(true),
    range(interval.isReal() ? interval
                            : Math::Range<double>::Raw(0, 0)),
    unit(std::string{unit}),
    step(step ? *step : Math::Renard::R5().ceil(range.size() / 5.0))
{
	if (Math::Floating::is_zero(range.size()))
		this->step->value = 0;
	else if (this->step->value == 0)
		this->step->value = std::signbit(range.size()) ? -1 : 1;
	else if (std::signbit(this->step->value)
	         != std::signbit(range.size()))
		this->step->value *= -1;
}

bool MeasureAxis::operator==(const MeasureAxis &other) const
{
	return enabled == other.enabled && range == other.range
	    && step == other.step && unit == other.unit;
}

double MeasureAxis::origo() const
{
	if (range.size() == 0) return 0;
	return -range.getMin() / range.size();
}

MeasureAxis interpolate(const MeasureAxis &op0,
    const MeasureAxis &op1,
    double factor)
{
	MeasureAxis res;
	res.enabled = interpolate(op0.enabled, op1.enabled, factor);

	if (op0.enabled.get() && op1.enabled.get()) {
		constexpr auto MAX = std::numeric_limits<double>::max() / 2;
		using Math::Floating::is_zero;

		const auto s0 = op0.range.size();
		const auto s1 = op1.range.size();

		if (auto s0Zero = is_zero(s0), s1Zero = is_zero(s1);
		    s0Zero && s1Zero) {
			res.range = Math::Range<double>::Raw(
			    Math::interpolate(op0.range.getMin(),
			        op1.range.getMin(),
			        factor),
			    Math::interpolate(op0.range.getMax(),
			        op1.range.getMax(),
			        factor));
			res.step = interpolate(op0.step, op1.step, factor);
		}
		else if (s1Zero) {
			auto size = factor == 1.0 ? MAX : s0 / (1 - factor);

			auto middleAt = Math::interpolate(
			    op0.range.rescale(op1.range.middle()),
			    0.0,
			    factor);

			res.range = Math::Range<double>::Raw(
			    op1.range.middle() - middleAt * size,
			    op1.range.middle()
			        + (factor == 1.0 ? 0.0 : (1 - middleAt) * size));

			auto step = op0.step.get() / s0 * size;
			auto max = std::copysign(MAX, step);

			res.step = interpolate(op0.step,
			    Anim::Interpolated{max},
			    Math::Range<double>::Raw(op0.step.get(), max)
			        .rescale(step));
		}
		else if (s0Zero) {
			auto size = factor == 0.0 ? MAX : s1 / factor;

			auto middleAt = Math::interpolate(0.0,
			    op1.range.rescale(op0.range.middle()),
			    factor);

			res.range = Math::Range<double>::Raw(
			    op0.range.middle() - middleAt * size,
			    op0.range.middle()
			        + (factor == 0.0 ? 0.0 : (1 - middleAt) * size));

			auto step = op1.step.get() / s1 * size;
			auto max = std::copysign(MAX, step);

			res.step = interpolate(op1.step,
			    Anim::Interpolated{max},
			    Math::Range<double>::Raw(op1.step.get(), max)
			        .rescale(step));
		}
		else {
			auto s0Inv = 1 / s0;
			auto s1Inv = 1 / s1;

			const auto interp =
			    Math::interpolate(s0Inv, s1Inv, factor);

			const auto size = is_zero(interp) ? MAX : 1 / interp;

			res.range = Math::Range<double>::Raw(
			    Math::interpolate(op0.range.getMin() * s0Inv,
			        op1.range.getMin() * s1Inv,
			        factor)
			        * size,
			    Math::interpolate(op0.range.getMax() * s0Inv,
			        op1.range.getMax() * s1Inv,
			        factor)
			        * size);

			auto step = Math::interpolate(op0.step.get() * s0Inv,
			                op1.step.get() * s1Inv,
			                factor)
			          * size;

			if (auto op0sign = std::signbit(op0.step.get());
			    op0sign == std::signbit(op1.step.get()))
				res.step = interpolate(op0.step,
				    op1.step,
				    Math::Range<double>::Raw(op0.step.get(),
				        op1.step.get())
				        .rescale(step));
			else if (auto max = std::copysign(MAX, step);
			         op0sign == std::signbit(step))
				res.step = interpolate(op0.step,
				    Anim::Interpolated{max},
				    Math::Range<double>::Raw(op0.step.get(), max)
				        .rescale(step));
			else
				res.step = interpolate(op1.step,
				    Anim::Interpolated{max},
				    Math::Range<double>::Raw(op1.step.get(), max)
				        .rescale(step));
		}

		res.unit = interpolate(op0.unit, op1.unit, factor);
	}
	else if (op0.enabled.get()) {
		res.range = op0.range;
		res.step = op0.step;
		res.unit = op0.unit;
	}
	else if (op1.enabled.get()) {
		res.range = op1.range;
		res.step = op1.step;
		res.unit = op1.unit;
	}

	return res;
}
bool DimensionAxis::add(const Data::SliceIndex &index,
    double value,
    const Math::Range<double> &range,
    bool merge,
    bool label)
{
	this->enabled = true;

	if (merge) {
		if (auto it = values.find(index); it != values.end()) {
			it->second.range.include(range);
			return false;
		}
	}
	values.emplace(std::piecewise_construct,
	    std::tuple{index},
	    std::tuple{range, value, index.value, label});

	return true;
}

bool DimensionAxis::operator==(const DimensionAxis &other) const
{
	return enabled == other.enabled && values == other.values;
}

bool DimensionAxis::setLabels(double step)
{
	bool hasLabel{};
	step = std::max(step, 1.0, Math::Floating::less);
	double currStep = 0.0;

	std::multimap<double, std::reference_wrapper<Item>> reorder;
	for (auto &ref : values)
		reorder.emplace(ref.second.range.getMin(), ref.second);

	for (int curr{};
	     Item & item : std::ranges::views::values(reorder)) {
		if (++curr <= currStep) continue;
		currStep += step;
		item.label = item.categoryValue;
		hasLabel = true;
	}
	return hasLabel;
}

DimensionAxis interpolate(const DimensionAxis &op0,
    const DimensionAxis &op1,
    double factor)
{
	DimensionAxis res;

	for (const auto &[slice, item] : op0.values) {
		res.enabled = true;
		res.values.emplace(std::piecewise_construct,
		    std::tuple{slice},
		    std::forward_as_tuple(item, true, 1 - factor));
	}

	for (const auto &[slice, item] : op1.values) {
		res.enabled = true;
		auto [resIt, end] = res.values.equal_range(slice);

		while (resIt != end && resIt->second.end) { ++resIt; }

		if (resIt == end) {
			res.values.emplace_hint(resIt,
			    std::piecewise_construct,
			    std::tuple{slice},
			    std::forward_as_tuple(item, false, factor));
		}
		else {
			resIt->second.end = true;

			resIt->second.range =
			    Math::interpolate(resIt->second.range,
			        item.range,
			        factor);

			resIt->second.colorBase =
			    interpolate(resIt->second.colorBase,
			        item.colorBase,
			        factor);

			resIt->second.label =
			    interpolate(resIt->second.label, item.label, factor);

			resIt->second.value =
			    Math::interpolate(resIt->second.value,
			        item.value,
			        factor);

			resIt->second.weight += item.weight * factor;
		}
	}

	return res;
}

}
