#include "axis.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <functional>
#include <limits>
#include <map>
#include <optional>
#include <set>
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

#include "colorbase.h"

namespace Vizzu::Gen
{

const Axis &Axises::empty()
{
	static const Axis empty;
	return empty;
}

void Axises::addLegendInterpolation(double legendFactor,
    LegendId legendType,
    const Axis &source,
    const Axis &target,
    double factor)
{
	if (&source == &empty() && &target == &empty()) return;
	using Math::Niebloid::interpolate;

	if (((source.measure.enabled.get()
	         && target.measure.enabled.get())
	        || (!source.dimension.empty()
	            && !target.dimension.empty()))
	    && source.seriesName() != target.seriesName()) {
		if (!leftLegend[0]) leftLegend[0].emplace(legendType);
		if (!leftLegend[1]) leftLegend[1].emplace(legendType);

		leftLegend[0]->calc = interpolate(source, empty(), factor);
		leftLegend[1]->calc = interpolate(empty(), target, factor);
		return;
	}

	auto second = leftLegend[0] && leftLegend[0]->type != legendType;
	auto &legendObj = leftLegend[second];
	if (!legendObj) legendObj.emplace(legendType);
	legendObj->calc = interpolate(source, target, factor);
	++legendObj->interpolated;

	if (!second) legendFactor = 1.0 - legendFactor;

	if (auto sameInterpolated =
	        leftLegend[0] && leftLegend[1]
	        && leftLegend[0]->interpolated
	               == leftLegend[1]->interpolated
	        && leftLegend[0]->calc.seriesName()
	               == leftLegend[1]->calc.seriesName();
	    sameInterpolated && !leftLegend[0]->calc.dimension.empty()
	    && !leftLegend[1]->calc.dimension.empty()) {

		leftLegend[1]->calc.dimension =
		    leftLegend[0]->calc.dimension =
		        interpolate(leftLegend[0]->calc.dimension,
		            leftLegend[1]->calc.dimension,
		            legendFactor);
	}
	else if (sameInterpolated
	         && leftLegend[0]->calc.measure.enabled.factor(true) > 0
	         && leftLegend[1]->calc.measure.enabled.factor(true)
	                > 0) {

		leftLegend[0]->calc.measure.enabled =
		    ::Anim::Interpolated{true};
		leftLegend[1]->calc.measure.enabled =
		    ::Anim::Interpolated{true};

		leftLegend[1]->calc.measure = leftLegend[0]->calc.measure =
		    interpolate(leftLegend[0]->calc.measure,
		        leftLegend[1]->calc.measure,
		        legendFactor);
	}
}

Geom::Point Axises::origo() const
{
	return {at(AxisId::x).measure.origo(),
	    at(AxisId::y).measure.origo()};
}

MeasureAxis::MeasureAxis(const Math::Range<> &interval,
    std::string series,
    const std::string_view &unit,
    const std::optional<double> &step) :
    enabled(true),
    range(interval.isReal() ? interval : Math::Range<>::Raw({}, {})),
    series(std::move(series)),
    unit(std::string{unit}),
    step(step ? *step : Math::Renard::R5().ceil(range.size() / 5.0))
{
	if (Math::Floating::is_zero(range.size()))
		this->step->value = 0;
	else if (std::signbit(this->step->value)
	         != std::signbit(range.size()))
		this->step->value *= -1;
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
	res.series = op0.series;

	if (op0.enabled.get() && op1.enabled.get()) {
		constexpr auto MAX = std::numeric_limits<double>::max() / 2;
		using Math::Floating::is_zero;

		const auto s0 = op0.range.size();
		const auto s1 = op1.range.size();

		if (auto s0Zero = is_zero(s0), s1Zero = is_zero(s1);
		    s0Zero && s1Zero) {
			res.range = Math::Range<>::Raw(
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

			res.range = Math::Range<>::Raw(op1.range.middle()
			                                   - middleAt * size,
			    op1.range.middle()
			        + (factor == 1.0 ? 0.0 : (1 - middleAt) * size));

			auto step = op0.step.get() / s0 * size;
			auto max = std::copysign(MAX, step);

			res.step = interpolate(op0.step,
			    Anim::Interpolated{max},
			    Math::Range<>::Raw(op0.step.get(), max)
			        .rescale(step));
		}
		else if (s0Zero) {
			auto size = factor == 0.0 ? MAX : s1 / factor;

			auto middleAt = Math::interpolate(0.0,
			    op1.range.rescale(op0.range.middle()),
			    factor);

			res.range = Math::Range<>::Raw(op0.range.middle()
			                                   - middleAt * size,
			    op0.range.middle()
			        + (factor == 0.0 ? 0.0 : (1 - middleAt) * size));

			auto step = op1.step.get() / s1 * size;
			auto max = std::copysign(MAX, step);

			res.step = interpolate(op1.step,
			    Anim::Interpolated{max},
			    Math::Range<>::Raw(op1.step.get(), max)
			        .rescale(step));
		}
		else {
			auto s0Inv = 1 / s0;
			auto s1Inv = 1 / s1;

			const auto interp =
			    Math::interpolate(s0Inv, s1Inv, factor);

			const auto size = is_zero(interp) ? MAX : 1 / interp;

			res.range = Math::Range<>::Raw(
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
				    Math::Range<>::Raw(op0.step.get(), op1.step.get())
				        .rescale(step));
			else if (auto max = std::copysign(MAX, step);
			         op0sign == std::signbit(step))
				res.step = interpolate(op0.step,
				    Anim::Interpolated{max},
				    Math::Range<>::Raw(op0.step.get(), max)
				        .rescale(step));
			else
				res.step = interpolate(op1.step,
				    Anim::Interpolated{max},
				    Math::Range<>::Raw(op1.step.get(), max)
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
		res.series = op1.series;
		res.range = op1.range;
		res.step = op1.step;
		res.unit = op1.unit;
	}

	return res;
}
bool DimensionAxis::add(const Data::SliceIndex &index,
    const Math::Range<> &range,
    const std::optional<std::uint32_t> &position,
    const std::optional<ColorBase> &color,
    bool label,
    bool merge)
{
	auto [it, end] = values.equal_range(index);
	if (merge) {
		if (it != end) {
			it->second.range.include(range);
			if (auto &col = it->second.colorBase;
			    col.hasOneValue() && color && col->value.isDiscrete()
			    && color->isDiscrete()
			    && col->value.getIndex() == color->getIndex())
				col->value.setLightness(it->second.range.middle());

			return false;
		}
	}
	else
		while (it != end)
			if (it++->second.range == range) return false;
	values.emplace(std::piecewise_construct,
	    std::tuple{index},
	    std::tuple{range, position, color, label});

	return true;
}

bool DimensionAxis::setLabels(double step)
{
	auto hasLabel = false;
	step = std::max(step, 1.0, Math::Floating::less);
	auto currStep = 0.0;

	using SortedItems =
		std::multiset<std::reference_wrapper<Item>, decltype(
			[] (Item& lhs, Item &rhs)
			{
				return Math::Floating::less(lhs.range.getMin(), rhs.range.getMin());
			})>;

	for (auto curr = int{};
	     auto &&item : SortedItems{begin(), end()}) {
		if (++curr <= currStep) continue;
		currStep += step;
		item.get().label = true;
		hasLabel = true;
	}
	return hasLabel;
}

DimensionAxis interpolate(const DimensionAxis &op0,
    const DimensionAxis &op1,
    double factor)
{
	DimensionAxis res;

	res.factor = factor;

	for (auto first1 = op0.values.begin(),
	          first2 = op1.values.begin(),
	          last1 = op0.values.end(),
	          last2 = op1.values.end();
	     first1 != last1 || first2 != last2;)
		if (first2 == last2
		    || (first1 != last1 && first1->first < first2->first)) {
			res.values.emplace(std::piecewise_construct,
			    std::tuple{first1->first},
			    std::forward_as_tuple(first1->second, true));
			++first1;
		}
		else if (first1 == last1 || first2->first < first1->first) {
			res.values.emplace(std::piecewise_construct,
			    std::tuple{first2->first},
			    std::forward_as_tuple(first2->second, false));
			++first2;
		}
		else {
			auto key = first1->first;
			auto to1 = op0.values.upper_bound(key);
			auto to2 = op1.values.upper_bound(key);

			while (first1 != to1 && first2 != to2)
				res.values.emplace(key,
				    interpolate(first1++->second,
				        first2++->second,
				        factor));

			for (const auto &latest = std::prev(to2)->second;
			     first1 != to1;
			     ++first1)
				res.values
				    .emplace(key,
				        interpolate(first1->second, latest, factor))
				    ->second.end = false;

			for (const auto &latest = std::prev(to1)->second;
			     first2 != to2;
			     ++first2)
				res.values
				    .emplace(key,
				        interpolate(latest, first2->second, factor))
				    ->second.start = false;
		}

	return res;
}

DimensionAxis::Item interpolate(const DimensionAxis::Item &op0,
    const DimensionAxis::Item &op1,
    double factor)
{
	using Math::Niebloid::interpolate;
	DimensionAxis::Item res;
	res.start = res.end = true;
	res.range = interpolate(op0.range, op1.range, factor);
	res.colorBase = interpolate(op0.colorBase, op1.colorBase, factor);
	res.label = interpolate(op0.label, op1.label, factor);
	res.position = interpolate(op0.position, op1.position, factor);
	return res;
}

}
