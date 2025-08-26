#include "axis.h"

#include <__compare/compare_three_way.h>
#include <algorithm>
#include <cmath>
#include <compare>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <limits>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>

#include "base/alg/merge.h"
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
	    && source.seriesName() != target.seriesName()
	    && !DimensionAxis::commonDimensionParts(
	        source.dimension.getValues(),
	        target.dimension.getValues())) {
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
	        && (leftLegend[0]->calc.seriesName()
	                == leftLegend[1]->calc.seriesName()
	            || DimensionAxis::commonDimensionParts(
	                leftLegend[0]->calc.dimension.getValues(),
	                leftLegend[1]->calc.dimension.getValues()));
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
std::size_t DimensionAxis::commonDimensionParts(const Values &lhs,
    const Values &rhs)
{
	return lhs.empty() || rhs.empty()
	         ? 0
	         : std::ranges::mismatch(lhs.begin()->first,
	               rhs.begin()->first,
	               [](const auto &lhsItem, const auto &rhsItem)
	               {
		               return lhsItem.column == rhsItem.column;
	               }).in1
	               - lhs.begin()->first.begin();
}

MeasureAxis::MeasureAxis(const Math::Range<> &interval,
    std::string &&series,
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
bool DimensionAxis::add(const std::vector<Data::SliceIndex> &index,
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
	res.hasMarker = !Math::Floating::is_zero(
	    Math::Niebloid::interpolate(op0.hasMarker,
	        op1.hasMarker,
	        factor));
	using Val = DimensionAxis::Values::value_type;

	std::size_t commonSize =
	    DimensionAxis::commonDimensionParts(op0.getValues(),
	        op1.getValues());
	std::size_t maxSize = std::max(
	    op0.values.empty() ? 0 : op0.values.begin()->first.size(),
	    op1.values.empty() ? 0 : op1.values.begin()->first.size());

	const Val *latest1{};
	const Val *latest2{};

	auto merger = [&](const Val &lhs,
	                  const Val &rhs,
	                  const Val::first_type *key = nullptr) -> Val
	{
		printf("%s to %s\n",
		    DimensionAxis::mergedLabels(lhs.first).c_str(),
		    DimensionAxis::mergedLabels(rhs.first).c_str());
		return {key ? *key : lhs.first,
		    interpolate(lhs.second, rhs.second, factor)};
	};

	auto needMerge = [&](const Val &lhs, const Val &rhs)
	{
		latest1 = std::addressof(lhs);
		latest2 = std::addressof(rhs);
		return commonSize == maxSize || commonSize == 0;
	};

	auto comparator = [&](const auto &lhs, const auto &rhs)
	{
		if (commonSize == maxSize || commonSize == 0)
			return std::compare_three_way{}(lhs, rhs);
		return std::lexicographical_compare_three_way(lhs.begin(),
		    lhs.begin() + commonSize,
		    rhs.begin(),
		    rhs.begin() + commonSize);
	};

	auto &&one_side =
	    [&](bool first,
	        DimensionAxis::Item::PosType DimensionAxis::Item::*pos,
	        const Val *&paramOther)
	{
		return [&, first, pos](const Val &val) -> Val
		{
			if (paramOther
			    && std::is_eq(
			        comparator(paramOther->first, val.first))) {
				auto &&res = first
				               ? merger(val, *paramOther)
				               : merger(*paramOther, val, &val.first);
				(res.second.*pos).makeAuto();
				return res;
			}
			return {val.first, {val.second, first}};
		};
	};

	Alg::merge(op0.values,
	    op1.values,
	    res.values,
	    Alg::merge_args{.projection = &Val::first,
	        .comparator = comparator,
	        .transformer_1 =
	            one_side(true, &DimensionAxis::Item::endPos, latest2),
	        .transformer_2 = one_side(false,
	            &DimensionAxis::Item::startPos,
	            latest1),
	        .need_merge = needMerge,
	        .merger = merger});

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

	using PartPair = SplitAxis::Parts::value_type;

	auto needMerge = op0.seriesName() == op1.seriesName()
	              && op0.measure.unit == op1.measure.unit;

	auto &&merger = [](double factor)
	{
		return [factor](const PartPair &lhs,
		           const PartPair &rhs) -> PartPair
		{
			return {lhs.first,
			    interpolate(lhs.second, rhs.second, factor)};
		};
	};

	using MergerType = decltype(merger(0.0));
	auto &&one_side = [](const MergerType &merger,
	                      bool needOther,
	                      const Math::Range<>::Transform &transform)
	{
		return [&merger, &transform, needOther](
		           const PartPair &val) -> PartPair
		{
			if (needOther)
				return merger(val,
				    {{},
				        {0.0,
				            (Math::Range<>{0, 1} * transform),
				            (val.second.measureRange * 1.000001
				                * transform)}});
			return merger(val,
			    {{},
			        {0.0,
			            val.second.range,
			            val.second.measureRange}});
		};
	};

	Alg::merge(op0.parts,
	    op1.parts,
	    res.parts,
	    Alg::merge_args{.projection = &PartPair::first,
	        .transformer_1 = one_side(merger(factor),
	            needMerge && op1.parts.empty(),
	            (op0.measure.range - op0.measure.range.min)
	                / (op1.measure.range - op1.measure.range.min)),
	        .transformer_2 = one_side(merger(1 - factor),
	            needMerge && op0.parts.empty(),
	            (op1.measure.range - op1.measure.range.min)
	                / (op0.measure.range - op0.measure.range.min)),
	        .need_merge = {needMerge},
	        .merger = merger(factor)});

	if (op0.parts.empty() != op1.parts.empty()
	    && (op0.dimension.hasMarker || op0.measure.enabled.get())
	    && (op1.dimension.hasMarker || op1.measure.enabled.get())) {
		if (needMerge) {
			const auto &ref =
			    (op0.parts.empty() ? op1.parts : op0.parts)
			        .begin()
			        ->second.range;
			res.parts
			    .insert(
			        merger(op0.parts.empty()
			                   ? 1 - factor
			                   : factor)({{}, {0.0, ref}}, {{}, {}}))
			    ->second.unique = true;
		}
		else
			res.parts
			    .insert({{},
			        {.weight =
			                op0.parts.empty() ? 1 - factor : factor}})
			    ->second.unique = true;
	}

	return res;
}

}
