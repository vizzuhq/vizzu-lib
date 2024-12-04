#include "axis.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <functional>
#include <iterator>
#include <limits>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>

#include "base/alg/union_foreach.h"
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
    range(interval.isReal() ? interval : Math::Range<>{{}, {}}),
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
	return -range.min / range.size();
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
			res.range = {Math::interpolate(op0.range.min,
			                 op1.range.min,
			                 factor),
			    Math::interpolate(op0.range.max,
			        op1.range.max,
			        factor)};
			res.step = interpolate(op0.step, op1.step, factor);
		}
		else if (s1Zero) {
			auto size = factor == 1.0 ? MAX : s0 / (1 - factor);

			auto middleAt = Math::interpolate(
			    op0.range.rescale(op1.range.middle()),
			    0.0,
			    factor);

			res.range = {op1.range.middle() - middleAt * size,
			    op1.range.middle()
			        + (factor == 1.0 ? 0.0 : (1 - middleAt) * size)};

			auto step = op0.step.get() / s0 * size;
			auto max = std::copysign(MAX, step);

			res.step = interpolate(op0.step,
			    Anim::Interpolated{max},
			    Math::Range<>{op0.step.get(), max}.rescale(step));
		}
		else if (s0Zero) {
			auto size = factor == 0.0 ? MAX : s1 / factor;

			auto middleAt = Math::interpolate(0.0,
			    op1.range.rescale(op0.range.middle()),
			    factor);

			res.range = {op0.range.middle() - middleAt * size,
			    op0.range.middle()
			        + (factor == 0.0 ? 0.0 : (1 - middleAt) * size)};

			auto step = op1.step.get() / s1 * size;
			auto max = std::copysign(MAX, step);

			res.step = interpolate(op1.step,
			    Anim::Interpolated{max},
			    Math::Range<>{op1.step.get(), max}.rescale(step));
		}
		else {
			auto s0Inv = 1 / s0;
			auto s1Inv = 1 / s1;

			const auto interp =
			    Math::interpolate(s0Inv, s1Inv, factor);

			const auto size = is_zero(interp) ? MAX : 1 / interp;

			res.range = {Math::interpolate(op0.range.min * s0Inv,
			                 op1.range.min * s1Inv,
			                 factor)
			                 * size,
			    Math::interpolate(op0.range.max * s0Inv,
			        op1.range.max * s1Inv,
			        factor)
			        * size};

			auto step = Math::interpolate(op0.step.get() * s0Inv,
			                op1.step.get() * s1Inv,
			                factor)
			          * size;

			if (auto op0sign = std::signbit(op0.step.get());
			    op0sign == std::signbit(op1.step.get()))
				res.step = interpolate(op0.step,
				    op1.step,
				    Math::Range<>{op0.step.get(), op1.step.get()}
				        .rescale(step));
			else if (auto max = std::copysign(MAX, step);
			         op0sign == std::signbit(step))
				res.step = interpolate(op0.step,
				    Anim::Interpolated{max},
				    Math::Range<>{op0.step.get(), max}.rescale(step));
			else
				res.step = interpolate(op1.step,
				    Anim::Interpolated{max},
				    Math::Range<>{op1.step.get(), max}.rescale(step));
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
    std::uint32_t position,
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

	for (auto curr = int{}; auto &&item : sortedItems()) {
		if (auto mid = item.get().range.middle();
		    std::signbit(mid) || mid > 1.0 || ++curr <= currStep)
			continue;
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
	using Ptr = DimensionAxis::Values::const_pointer;
	Alg::union_foreach(
	    op0.values,
	    op1.values,
	    [&res](Ptr v1, Ptr v2, Alg::union_call_t type)
	    {
		    if (!v2)
			    res.values.emplace(std::piecewise_construct,
			        std::tuple{v1->first},
			        std::forward_as_tuple(v1->second, true));
		    else if (!v1)
			    res.values.emplace(std::piecewise_construct,
			        std::tuple{v2->first},
			        std::forward_as_tuple(v2->second, false));
		    else if (auto &&val = res.values
		                              .emplace(v1->first,
		                                  interpolate(v1->second,
		                                      v2->second,
		                                      res.factor))
		                              ->second;
		             type == Alg::union_call_t::only_left)
			    val.endPos.makeAuto();
		    else if (type == Alg::union_call_t::only_right)
			    val.startPos.makeAuto();
	    },
	    res.values.value_comp());

	return res;
}

DimensionAxis::Item interpolate(const DimensionAxis::Item &op0,
    const DimensionAxis::Item &op1,
    double factor)
{
	using Math::Niebloid::interpolate;
	DimensionAxis::Item res;
	res.startPos = op0.startPos;
	res.endPos = op1.endPos;
	res.range = interpolate(op0.range, op1.range, factor);
	res.colorBase = interpolate(op0.colorBase, op1.colorBase, factor);
	res.label = interpolate(op0.label, op1.label, factor);
	return res;
}

SplitAxis
interpolate(const SplitAxis &op0, const SplitAxis &op1, double factor)
{
	using Math::Niebloid::interpolate;
	SplitAxis res;
	static_cast<Axis &>(res) =
	    interpolate(static_cast<const Axis &>(op0),
	        static_cast<const Axis &>(op1),
	        factor);
	if (!op0.parts.empty() && !op1.parts.empty()) {
		using PartPair = const decltype(res.parts)::value_type;
		Alg::union_foreach(
		    op0.parts,
		    op1.parts,
		    [&res, &factor](PartPair *lhs,
		        PartPair *rhs,
		        Alg::union_call_t type)
		    {
			    switch (type) {
			    case Alg::union_call_t::only_left: {
				    auto from = lhs->second.range.getMin();
				    res.parts[lhs->first] = {
				        .weight = interpolate(lhs->second.weight,
				            0.0,
				            factor),
				        .range = interpolate(lhs->second.range,
				            Math::Range<>::Raw(from, from),
				            factor)};
				    break;
			    }
			    case Alg::union_call_t::only_right: {
				    auto from = rhs->second.range.getMin();
				    res.parts[rhs->first] = {
				        .weight = interpolate(0.0,
				            rhs->second.weight,
				            factor),
				        .range = interpolate(
				            Math::Range<>::Raw(from, from),
				            rhs->second.range,
				            factor)};
				    break;
			    }
			    default:
			    case Alg::union_call_t::both: {
				    res.parts[lhs->first] =
				        interpolate(lhs->second, rhs->second, factor);
				    break;
			    }
			    }
		    },
		    res.parts.value_comp());
	}
	else if (!op0.parts.empty()) {
		auto begin = op0.parts.begin();
		res.parts[begin->first] = {
		    .weight = interpolate(begin->second.weight, 1.0, factor),
		    .range = interpolate(begin->second.range,
		        Math::Range<>::Raw(0, 1),
		        factor)};
		while (++begin != op0.parts.end()) {
			res.parts[begin->first] = {
			    .weight =
			        interpolate(begin->second.weight, 0.0, factor),
			    .range = interpolate(begin->second.range,
			        Math::Range<>::Raw(0, 1),
			        factor)};
		}
	}
	else if (!op1.parts.empty()) {
		auto begin = op1.parts.begin();
		res.parts[begin->first] = {
		    .weight = interpolate(1.0, begin->second.weight, factor),
		    .range = interpolate(Math::Range<>::Raw(0, 1),
		        begin->second.range,
		        factor)};
		while (++begin != op1.parts.end()) {
			res.parts[begin->first] = {
			    .weight =
			        interpolate(0.0, begin->second.weight, factor),
			    .range = interpolate(Math::Range<>::Raw(0, 1),
			        begin->second.range,
			        factor)};
		}
	}

	return res;
}

}
