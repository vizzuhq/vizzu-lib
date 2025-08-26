#include "drawaxes.h"

#include <algorithm>
#include <cmath>
#include <numbers>
#include <optional>
#include <ranges>
#include <string>
#include <utility>

#include "base/anim/interpolated.h"
#include "base/geom/affinetransform.h"
#include "base/geom/line.h"
#include "base/geom/orientation.h"
#include "base/geom/point.h"
#include "base/geom/transformedrect.h"
#include "base/gfx/colortransform.h"
#include "base/gfx/font.h"
#include "base/math/floating.h"
#include "base/math/fuzzybool.h"
#include "base/math/interpolation.h"
#include "base/math/range.h"
#include "base/math/renard.h"
#include "base/refl/auto_enum.h"
#include "base/type/booliter.h"
#include "chart/generator/axis.h"
#include "chart/generator/plot.h" // NOLINT(misc-include-cleaner)
#include "chart/main/events.h"
#include "chart/main/style.h"
#include "chart/options/channel.h"
#include "dataframe/old/types.h"

#include "drawguides.h"
#include "drawinterlacing.h"
#include "drawlabel.h"
#include "orientedlabel.h"
#include "renderedchart.h"

namespace Vizzu::Draw
{

void DrawAxes::drawGeometries() const
{
	auto origo = this->origo();
	for (auto &&xSplit : std::views::values(splits[Gen::AxisId::x]))
		for (auto &&ySplit :
		    std::views::values(splits[Gen::AxisId::y])) {
			auto weight =
			    Math::FuzzyBool::And(xSplit.weight, ySplit.weight);
			if (Math::Floating::is_zero(weight)) continue;

			const Geom::AffineTransform tr{xSplit.range.size(),
			    0.0,
			    xSplit.range.min,
			    0.0,
			    ySplit.range.size(),
			    ySplit.range.min};

			DrawInterlacing{*this}.drawGeometries(Gen::AxisId::y,
			    ySplit.measureRange,
			    xSplit.measureRange,
			    tr,
			    weight);
			DrawInterlacing{*this}.drawGeometries(Gen::AxisId::x,
			    xSplit.measureRange,
			    ySplit.measureRange,
			    tr,
			    weight);

			if (ySplit.measureRange.includes(
			        origo.getCoord(orientation(Gen::AxisId::y))))
				drawAxis(Gen::AxisId::x,
				    xSplit.measureRange,
				    tr,
				    weight);
			if (xSplit.measureRange.includes(
			        origo.getCoord(orientation(Gen::AxisId::x))))
				drawAxis(Gen::AxisId::y,
				    ySplit.measureRange,
				    tr,
				    weight);

			DrawGuides{*this}.draw(Gen::AxisId::x,
			    xSplit.measureRange,
			    ySplit.measureRange,
			    tr,
			    weight);
			DrawGuides{*this}.draw(Gen::AxisId::y,
			    ySplit.measureRange,
			    xSplit.measureRange,
			    tr,
			    weight);
		}
}

void DrawAxes::drawLabels() const
{
	auto &&yValues = std::views::values(splits[Gen::AxisId::y]);
	for (auto &&ySplit : yValues) {
		const Geom::AffineTransform tr{1,
		    0.0,
		    0.0,
		    0.0,
		    ySplit.range.size(),
		    ySplit.range.min};
		DrawInterlacing{*this}.drawTexts(Gen::AxisId::y,
		    ySplit.measureRange,
		    tr,
		    ySplit.weight,
		    yValues.size() > 1 && !ySplit.unique);
	}

	auto &&xValues = std::views::values(splits[Gen::AxisId::x]);
	for (auto &&xSplit : xValues) {
		const Geom::AffineTransform tr{xSplit.range.size(),
		    0.0,
		    xSplit.range.min,
		    0.0,
		    1.0,
		    0.0};
		DrawInterlacing{*this}.drawTexts(Gen::AxisId::x,
		    xSplit.measureRange,
		    tr,
		    xSplit.weight,
		    xValues.size() > 1 && !xSplit.unique);
	}

	drawDimensionLabels(Gen::AxisId::x);
	drawDimensionLabels(Gen::AxisId::y);

	drawTitle(Gen::AxisId::x);
	drawTitle(Gen::AxisId::y);
}

const DrawAxes &&DrawAxes::init() &&
{
	for (auto axisIndex : Refl::enum_values<Gen::AxisId>()) {
		const auto &axis = plot->axises.at(axisIndex);

		const static Gen::SplitAxis::Parts oneSized{
		    {{}, Gen::SplitAxis::Part{}}};
		splits[axisIndex] =
		    axis.parts.empty() ? oneSized : axis.parts;

		auto measEnabled = axis.measure.enabled.combine<double>();
		auto &intervals = this->intervals[axisIndex];
		auto &separators = this->separators[axisIndex];
		const auto &guides = plot->guides.at(axisIndex);

		for (auto &&[index, item] : axis.dimension.getValues()) {
			auto weight = item.weight(axis.dimension.factor);
			if (Math::Floating::is_zero(weight)) continue;

			auto needInterlacing =
			    measEnabled == 0.0
			    || Math::FuzzyBool::And(Math::FuzzyBool::more(weight),
			           guides.interlacings.more())
			           != false;

			intervals.emplace_back(item.range.positive(),
			    weight,
			    Math::FuzzyBool::And<double>(
			        Math::Niebloid::interpolate(
			            (item.startPos ? *item.startPos
			                           : *item.endPos)
			                % 2,
			            (item.endPos ? *item.endPos : *item.startPos)
			                % 2,
			            axis.dimension.factor),
			        needInterlacing),
			    Interval::DimLabel{index,
			        item.label,
			        !item.startPos.isAuto(),
			        !item.endPos.isAuto()});

			auto needSeparators =
			    measEnabled == 0.0
			    || Math::FuzzyBool::And(Math::FuzzyBool::more(weight),
			           Math::FuzzyBool::Or(guides.axisSticks.more(),
			               guides.axisGuides.more()))
			           != false;

			if (auto sepWeight = Math::Niebloid::interpolate(
			        !item.startPos.isAuto() && *item.startPos,
			        !item.endPos.isAuto() && *item.endPos,
			        axis.dimension.factor);
			    needSeparators && sepWeight > 0 && item.range.min > 0)
				separators.emplace_back(item.range.min, sepWeight);
		}

		if (measEnabled == 0.0) continue;
		auto step = axis.measure.step.combine();

		using Math::Floating::less;

		auto &&[min, max] = std::minmax(
		    axis.measure.step.get_or_first(::Anim::first).value,
		    axis.measure.step.get_or_first(::Anim::second).value,
		    less);

		auto stepHigh =
		    std::clamp(Math::Renard::R5().ceil(step), min, max, less);
		auto stepLow = std::clamp(Math::Renard::R5().floor(step),
		    min,
		    max,
		    less);

		if (Math::Floating::is_zero(axis.measure.range.size()))
			step = stepHigh = stepLow = 1.0;

		if (stepHigh == step || stepLow == step)
			generateMeasure(axisIndex, step, measEnabled);
		else {
			auto highWeight =
			    Math::Range<>{stepLow, stepHigh}.rescale(step);

			generateMeasure(axisIndex,
			    stepLow,
			    (1.0 - highWeight) * measEnabled);
			generateMeasure(axisIndex,
			    stepHigh,
			    highWeight * measEnabled);
		}
	}

	return std::move(*this);
}

void DrawAxes::generateMeasure(Gen::AxisId axisIndex,
    double stepSize,
    double weight)
{
	auto orientation = !Gen::orientation(axisIndex);
	const auto &meas = getAxis(axisIndex).measure;
	auto rangeSize = meas.range.size();
	auto singleLabelRange = Math::Floating::is_zero(rangeSize);

	double stripWidth{};
	if (!singleLabelRange) {
		stripWidth = stepSize / rangeSize;
		if (stripWidth <= 0) return;
	}

	const auto origo = this->origo();
	auto axisBottom = origo.getCoord(!orientation) + stripWidth;

	auto iMin = static_cast<int>(
	    axisBottom > 0
	        ? std::floor(
	              -origo.getCoord(!orientation) / (2 * stripWidth))
	              * 2
	        : std::round((meas.range.min - stepSize) / stepSize));

	if (axisBottom + iMin * stripWidth + stripWidth < 0.0)
		iMin += 2
		      * static_cast<int>(std::ceil(
		          -(axisBottom + stripWidth) / (2 * stripWidth)));

	auto Transform = [&](int x)
	{
		return std::pair{iMin + x * 2,
		    axisBottom + (iMin + x * 2) * stripWidth};
	};
	constexpr static auto Predicate =
	    [](const std::pair<int, double> &x)
	{
		return x.second <= 1.0;
	};

	auto &separators = this->separators[axisIndex];
	auto &intervals = this->intervals[axisIndex];

	for (auto &&[i, bottom] :
	    std::views::iota(0) | std::views::transform(Transform)
	        | std::views::take_while(Predicate)) {
		if (!std::signbit(bottom)) {
			separators.emplace_back(bottom,
			    weight,
			    (i + 1) * stepSize);
		}

		if (!singleLabelRange) {
			intervals.emplace_back(
			    Math::Range<>{bottom, bottom + stripWidth}.positive(),
			    weight,
			    1.0);

			if (!Math::Floating::less(1.0, bottom + stripWidth)) {
				separators.emplace_back(bottom + stripWidth,
				    weight,
				    (i + 2) * stepSize);
			}
		}
		else
			break;
	}
}

Geom::Line DrawAxes::getAxisLine(Gen::AxisId axisIndex,
    const Math::Range<> &filter) const
{
	auto o = orientation(axisIndex);
	if (auto offset = this->origo().getCoord(!o);
	    Math::Range<>{0.0, 1.0}.includes(offset))
		return {Geom::Point::Coord(o, filter.min, offset),
		    Geom::Point::Coord(o, filter.max, offset)};
	return {};
}

void DrawAxes::drawAxis(Gen::AxisId axisIndex,
    const Math::Range<> &filter,
    const Geom::AffineTransform &tr,
    double w) const
{
	if (auto line = tr(getAxisLine(axisIndex, filter));
	    !line.isPoint()) {
		auto lineColor = *rootStyle.plot.getAxis(axisIndex).color
		               * Math::FuzzyBool::And<double>(w,
		                   plot->guides.at(axisIndex).axis);

		if (lineColor.isTransparent()) return;

		canvas.save();

		canvas.setLineColor(lineColor);
		canvas.setLineWidth(1.0);

		if (auto &&eventTarget = Events::Targets::axis(axisIndex);

		    rootEvents.draw.plot.axis.base->invoke(
		        Events::OnLineDrawEvent(*eventTarget,
		            {line, true}))) {
			painter.drawLine(line);
			renderedChart.emplace(Draw::Line{line, true},
			    std::move(eventTarget));
		}

		canvas.restore();
	}
}

Geom::Point DrawAxes::getTitleBasePos(Gen::AxisId axisIndex,
    ::Anim::InterpolateIndex index) const
{
	typedef Styles::AxisTitle::Position Pos;
	typedef Styles::AxisTitle::VPosition VPos;

	const auto &titleStyle = rootStyle.plot.getAxis(axisIndex).title;

	double orthogonal{0.0};

	switch (titleStyle.position->get_or_first(index).value) {
	default:
	case Pos::min_edge: break;
	case Pos::max_edge: orthogonal = 1.0; break;
	case Pos::axis:
		orthogonal = origo().getCoord(!orientation(axisIndex));
		break;
	}

	double parallel{0.0};

	switch (titleStyle.vposition->get_or_first(index).value) {
	default:
	case VPos::end: parallel = 1.0; break;
	case VPos::middle: parallel = 0.5; break;
	case VPos::begin: break;
	}

	return axisIndex == Gen::AxisId::x
	         ? Geom::Point{parallel, orthogonal}
	         : Geom::Point{orthogonal, parallel};
}

Geom::Point DrawAxes::getTitleOffset(Gen::AxisId axisIndex,
    ::Anim::InterpolateIndex index,
    bool fades) const
{
	const auto &titleStyle = rootStyle.plot.getAxis(axisIndex).title;

	auto calcSide = [](const auto &side)
	{
		typedef Styles::AxisTitle::Side Side;
		switch (side) {
		default:
		case Side::positive: return 1.0;
		case Side::negative: return -1.0;
		case Side::upon: return 0.0;
		}
	};

	auto orthogonal =
	    fades ? calcSide(titleStyle.side->get_or_first(index).value)
	          : titleStyle.side->combine(calcSide);

	auto calcVSide = [](const auto &side)
	{
		typedef Styles::AxisTitle::VSide Side;
		switch (side) {
		default:
		case Side::positive: return 1.0;
		case Side::negative: return -1.0;
		case Side::upon: return 0.0;
		}
	};

	auto parallel =
	    fades ? calcVSide(titleStyle.vside->get_or_first(index).value)
	          : titleStyle.vside->combine(calcVSide);

	return axisIndex == Gen::AxisId::x
	         ? Geom::Point{parallel, -orthogonal}
	         : Geom::Point{orthogonal, -parallel};
}

void DrawAxes::drawTitle(Gen::AxisId axisIndex,
    const Geom::AffineTransform &tr,
    double w) const
{
	const auto &titleString = getAxis(axisIndex).title;

	const auto &titleStyle = rootStyle.plot.getAxis(axisIndex).title;

	auto fades = titleStyle.position->maxIndex()
	          || titleStyle.vposition->maxIndex()
	          || titleString.maxIndex();

	for (auto &&index : Type::Bools{fades}) {
		auto title = titleString.get_or_first(index);
		if (title.value.empty()) continue;

		auto weight = Math::FuzzyBool::And(w,
		    title.weight,
		    titleStyle.position->get_or_first(index).weight,
		    titleStyle.vposition->get_or_first(index).weight);

		const Gfx::Font font(titleStyle);
		canvas.setFont(font);
		auto size = titleStyle.extendSize(
		    Gfx::ICanvas::textBoundary(font, title.value),
		    font.size);

		auto relCenter = getTitleBasePos(axisIndex, index);

		auto normal = Geom::Point::X(1.0);

		auto offset =
		    getTitleOffset(axisIndex, index, fades == ::Anim::second);

		auto posDir = coordSys.convertDirectionAt(
		    tr(Geom::Line{relCenter, relCenter + normal}));

		auto posAngle = posDir.getDirection().angle();

		canvas.save();

		Geom::AffineTransform transform(posDir.begin, 1.0, -posAngle);

		auto calcOrientation =
		    [&size](const Styles::AxisTitle::Orientation &orientation)
		{
			return Geom::Size{
			    isHorizontal(orientation) ? size : size.flip()};
		};

		auto angle =
		    -std::numbers::pi / 2.0
		    * (fades == ::Anim::second
		            ? !isHorizontal(
		                titleStyle.orientation->get_or_first(index)
		                    .value)
		            : titleStyle.orientation->factor(
		                Styles::AxisTitle::Orientation::vertical));

		auto orientedSize =
		    fades == ::Anim::second
		        ? calcOrientation(
		            titleStyle.orientation->get_or_first(index).value)
		        : titleStyle.orientation->combine(calcOrientation);

		auto center = offset * (orientedSize / 2.0);

		transform = transform
		          * Geom::AffineTransform(center, 1.0, angle)
		          * Geom::AffineTransform((orientedSize / -2.0));

		auto realAngle = Geom::Angle(-posAngle + angle).rad();
		auto upsideDown = realAngle > std::numbers::pi / 2.0
		               && realAngle < 3 * std::numbers::pi / 2.0;

		DrawLabel{{ctx()}}.draw(canvas,
		    Geom::TransformedRect{transform, Geom::Size{size}},
		    title.value,
		    titleStyle,
		    *rootEvents.draw.plot.axis.title,
		    Events::Targets::axisTitle(title.value, axisIndex),
		    {.colorTransform = Gfx::ColorTransform::Opacity(weight),
		        .flip = upsideDown});

		canvas.restore();
	}
}

void DrawAxes::drawDimensionLabels(Gen::AxisId axisIndex,
    const Geom::AffineTransform &tr,
    double w) const
{
	const auto &labelStyle = rootStyle.plot.getAxis(axisIndex).label;

	auto textColor = *labelStyle.color;
	if (textColor.isTransparent()) return;

	auto origo = this->origo();
	const auto &axis = getAxis(axisIndex).dimension;

	if (!axis.empty()) {
		canvas.setFont(Gfx::Font{labelStyle});
		const auto &enabled = plot->guides.at(axisIndex);

		for (const auto &interval : getIntervals(axisIndex)) {
			if (!interval.label) continue;
			drawDimensionLabel(axisIndex,
			    origo,
			    interval,
			    tr,
			    Math::FuzzyBool::And<double>(w,
			        interval.weight,
			        enabled.labels));
		}
	}
}

void DrawAxes::drawDimensionLabel(Gen::AxisId axisIndex,
    const Geom::Point &origo,
    const Interval &interval,
    const Geom::AffineTransform &tr,
    double weight) const
{
	if (weight == 0) return;
	auto orientation = Gen::orientation(axisIndex);

	const auto &labelStyle = rootStyle.plot.getAxis(axisIndex).label;

	auto drawLabel = OrientedLabel{{ctx()}};
	labelStyle.position->visit(
	    [this,
	        &tr,
	        &axisIndex,
	        &drawLabel,
	        &labelStyle,
	        &interval,
	        &orientation,
	        &origo,
	        ident = Geom::Point::Ident(orientation),
	        normal = Geom::Point::Ident(!orientation),
	        &dimInfo = *interval.label,
	        &weight](::Anim::InterpolateIndex index,
	        const auto &position)
	    {
		    if (labelStyle.position->interpolates()
		        && !dimInfo.presentAt(index))
			    return;

		    Geom::Point refPos;

		    switch (position.value) {
			    using Pos = Styles::AxisLabel::Position;
		    case Pos::max_edge: refPos = normal; break;
		    case Pos::axis: refPos = origo.comp(!orientation); break;
		    default:
		    case Pos::min_edge: refPos = Geom::Point(); break;
		    }

		    auto relCenter = refPos + ident * interval.range.middle();

		    auto under =
		        labelStyle.position->interpolates()
		            ? labelStyle.side->get_or_first(index).value
		                  == Styles::AxisLabel::Side::negative
		            : labelStyle.side->factor(
		                Styles::AxisLabel::Side::negative);

		    auto draw = [&,
		                    posDir = coordSys
		                                 .convertDirectionAt(
		                                     tr(Geom::Line{relCenter,
		                                         relCenter + normal}))
		                                 .extend(1 - 2 * under)](
		                    const ::Anim::Weighted<bool> &str,
		                    double plusWeight = 1.0)
		    {
			    if (!str.value) return;
			    auto dimValues =
			        Gen::DimensionAxis::mergedLabels(dimInfo.index);
			    drawLabel.draw(canvas,
			        dimValues,
			        posDir,
			        labelStyle,
			        0,
			        Gfx::ColorTransform::Opacity(
			            Math::FuzzyBool::And(weight,
			                str.weight,
			                plusWeight)),
			        *rootEvents.draw.plot.axis.label,
			        Events::Targets::dimAxisLabel(dimInfo.index,
			            dimValues,
			            axisIndex));
		    };

		    if (dimInfo.presented.interpolates()
		        && !labelStyle.position->interpolates()) {
			    draw(dimInfo.presented.get_or_first(::Anim::first));
			    draw(dimInfo.presented.get_or_first(::Anim::second));
		    }
		    else
			    draw(dimInfo.presented.get_or_first(index),
			        !labelStyle.position->interpolates()
			            ? 1.0
			            : position.weight);
	    });
}

}