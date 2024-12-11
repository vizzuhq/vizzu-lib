#include "drawinterlacing.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iterator>
#include <map>
#include <utility>

#include "base/anim/interpolated.h"
#include "base/geom/affinetransform.h"
#include "base/geom/point.h"
#include "base/geom/rect.h"
#include "base/gfx/colortransform.h"
#include "base/math/floating.h"
#include "base/math/fuzzybool.h"
#include "base/math/interpolation.h"
#include "base/math/range.h"
#include "base/text/smartstring.h"
#include "base/type/booliter.h"
#include "chart/generator/plot.h" // NOLINT(misc-include-cleaner)
#include "chart/main/events.h"
#include "chart/options/channel.h"

#include "orientedlabel.h"
#include "renderedchart.h"

namespace Vizzu::Draw
{

void DrawInterlacing::drawGeometries(Gen::AxisId axisIndex,
    const Math::Range<> &filter,
    const Geom::AffineTransform &tr,
    double w) const
{
	const auto &guides = parent.plot->guides.at(axisIndex);
	const auto &axisStyle = parent.rootStyle.plot.getAxis(axisIndex);
	if (axisStyle.interlacing.color->isTransparent()
	    || guides.interlacings == false)
		return;

	auto otherWeights = getInterlacingWeights(!axisIndex);
	auto &&otherInterlacingColor =
	    *parent.rootStyle.plot.getAxis(!axisIndex).interlacing.color;

	parent.painter.setPolygonToCircleFactor(0);
	parent.painter.setPolygonStraightFactor(0);

	for (const auto &interval :
	    parent.getIntervals(axisIndex, filter)) {
		if (Math::Floating::is_zero(interval.isSecond)) continue;
		auto clippedBottom = std::max(interval.range.min,
		    filter.min,
		    Math::Floating::less);
		auto clippedSize = std::min(interval.range.max,
		                       filter.max,
		                       Math::Floating::less)
		                 - clippedBottom;

		auto rect = [&, orientation = orientation(axisIndex)](
		                const double &from,
		                const double &to)
		{
			return Geom::Rect{tr(Geom::Point::Coord(orientation,
			                      clippedBottom,
			                      from)),
			    tr(Geom::Size::Oriented(orientation,
			        clippedSize,
			        to - from))};
		};

		auto weight = Math::FuzzyBool::And<double>(w,
		    interval.weight,
		    interval.isSecond,
		    guides.interlacings);
		auto interlacingColor = *axisStyle.interlacing.color * weight;

		for (auto first = otherWeights.begin(),
		          next = std::next(first),
		          last = otherWeights.end();
		     next != last;
		     ++next, ++first) {
			if (Math::Floating::is_zero(first->second))
				drawInterlacing(axisIndex,
				    interlacingColor,
				    rect(first->first, next->first));
			else if (axisIndex == Gen::AxisId::y) {
				drawInterlacing(first->second > weight ? !axisIndex
				                                       : axisIndex,
				    getCrossingInterlacingColor(
				        *axisStyle.interlacing.color,
				        weight,
				        otherInterlacingColor,
				        first->second),
				    rect(first->first, next->first));
			}
		}
	}
}

void DrawInterlacing::drawTexts(Gen::AxisId axisIndex,
    const Math::Range<> &filter,
    const Geom::AffineTransform &tr,
    double w) const
{
	const auto &axis = parent.getAxis(axisIndex).measure;
	auto orientation = !Gen::orientation(axisIndex);
	auto origo = parent.origo().getCoord(orientation);
	const auto &guides = parent.plot->guides.at(axisIndex);
	const auto &axisStyle = parent.rootStyle.plot.getAxis(axisIndex);

	auto tickLength = axisStyle.ticks.length->get(
	    parent.coordSys.getRect().size.getCoord(orientation),
	    axisStyle.label.calculatedSize());

	auto needTick = tickLength > 0
	             && !axisStyle.ticks.color->isTransparent()
	             && guides.axisSticks != false
	             && *axisStyle.ticks.lineWidth > 0;

	auto needText = !axisStyle.label.color->isTransparent()
	             && guides.labels != false;

	if (!needText && !needTick) return;

	for (const auto &sep : parent.getSeparators(axisIndex, filter)) {
		auto tickPos =
		    Geom::Point::Coord(orientation, origo, sep.position);
		if (needText && sep.label)
			drawDataLabel(axis.enabled,
			    axisIndex,
			    tickPos,
			    tr,
			    *sep.label,
			    axis.unit,
			    Math::FuzzyBool::And<double>(w,
			        sep.weight,
			        guides.labels));

		if (needTick)
			drawSticks(tickLength,
			    *axisStyle.ticks.color
			        * Math::FuzzyBool::And<double>(w,
			            sep.weight,
			            guides.axisSticks),
			    axisIndex,
			    tickPos,
			    tr);
	}
}

void DrawInterlacing::drawInterlacing(Gen::AxisId axisIndex,
    const Gfx::Color &interlacingColor,
    const Geom::Rect &rect) const
{
	auto &canvas = parent.canvas;
	canvas.save();
	canvas.setLineColor(Gfx::Color::Transparent());
	canvas.setLineWidth(0);
	canvas.setBrushColor(interlacingColor);
	if (auto &&eventTarget =
	        Events::Targets::axisInterlacing(axisIndex);
	    parent.rootEvents.draw.plot.axis.interlacing->invoke(
	        Events::OnRectDrawEvent(*eventTarget, {rect, true}))) {
		parent.painter.drawPolygon(rect.points());
		parent.renderedChart.emplace(Rect{rect, true},
		    std::move(eventTarget));
	}
	canvas.restore();
}

void DrawInterlacing::drawDataLabel(
    const ::Anim::Interpolated<bool> &axisEnabled,
    Gen::AxisId axisIndex,
    const Geom::Point &tickPos,
    const Geom::AffineTransform &tr,
    double value,
    const ::Anim::String &unit,
    double alpha) const
{
	auto orientation = !Gen::orientation(axisIndex);
	const auto &labelStyle =
	    parent.rootStyle.plot.getAxis(axisIndex).label;

	auto drawLabel = OrientedLabel{{parent.ctx()}};
	auto interpolates =
	    labelStyle.position->maxIndex() || unit.maxIndex();

	auto &&normal = Geom::Point::Ident(orientation);
	for (auto &&index : Type::Bools{interpolates}) {
		if (labelStyle.position->interpolates()
		    && !axisEnabled.get_or_first(index).value)
			continue;
		auto &&position = labelStyle.position->get_or_first(index);

		Geom::Point refPos = tickPos;

		switch (position.value) {
			using Pos = Styles::AxisLabel::Position;
		case Pos::min_edge: refPos.getCoord(orientation) = 0.0; break;
		case Pos::max_edge: refPos.getCoord(orientation) = 1.0; break;
		default: break;
		}

		auto under = labelStyle.position->interpolates()
		               ? labelStyle.side->get_or_first(index).value
		                     == Styles::AxisLabel::Side::negative
		               : labelStyle.side->factor(
		                   Styles::AxisLabel::Side::negative);

		auto &&posDir = parent.coordSys
		                    .convertDirectionAt(tr(
		                        Geom::Line{refPos, refPos + normal}))
		                    .extend(1 - 2 * under);

		auto &&wUnit = unit.get_or_first(index);
		auto str = Text::SmartString::fromPhysicalValue(value,
		    *labelStyle.numberFormat,
		    static_cast<size_t>(*labelStyle.maxFractionDigits),
		    *labelStyle.numberScale,
		    wUnit.value);
		drawLabel.draw(parent.canvas,
		    str,
		    posDir,
		    labelStyle,
		    0,
		    Gfx::ColorTransform::Opacity(Math::FuzzyBool::And(alpha,
		        position.weight,
		        wUnit.weight)),
		    *parent.rootEvents.draw.plot.axis.label,
		    Events::Targets::measAxisLabel(str, axisIndex));
	}
}

void DrawInterlacing::drawSticks(double tickLength,
    const Gfx::Color &tickColor,
    Gen::AxisId axisIndex,
    const Geom::Point &tickPos,
    const Geom::AffineTransform &tr) const
{
	auto &canvas = parent.canvas;
	const auto &tickStyle =
	    parent.rootStyle.plot.getAxis(axisIndex).ticks;

	canvas.save();

	canvas.setLineColor(tickColor);
	canvas.setBrushColor(tickColor);

	canvas.setLineWidth(*tickStyle.lineWidth);

	auto tickLine = tickStyle.position->combine(
	    [tickLine = parent.coordSys
	                    .convertDirectionAt(tr(Geom::Line{tickPos,
	                        tickPos
	                            + Geom::Point::Coord(
	                                !orientation(axisIndex),
	                                -1.0)}))
	                    .segment(0, tickLength)](const auto &position)
	    {
		    switch (position) {
			    using enum Styles::Tick::Position;
		    default:
		    case outside: return tickLine;
		    case inside: return tickLine.segment(-1, 0);
		    case center: return tickLine.segment(-0.5, 0.5);
		    }
	    });

	if (auto &&eventTarget = Events::Targets::axisTick(axisIndex);
	    parent.rootEvents.draw.plot.axis.tick->invoke(
	        Events::OnLineDrawEvent(*eventTarget,
	            {tickLine, false}))) {
		canvas.line(tickLine);
		parent.renderedChart.emplace(Line{tickLine, false},
		    std::move(eventTarget));
	}

	canvas.restore();
}

std::map<double, double> DrawInterlacing::getInterlacingWeights(
    Gen::AxisId axisIndex) const
{
	std::map<double, double> weights{{0.0, 0.0}, {1.0, 0.0}};

	auto &&guides = parent.plot->guides.at(axisIndex);
	auto &&axisStyle = parent.rootStyle.plot.getAxis(axisIndex);
	if (axisStyle.interlacing.color->isTransparent()
	    || guides.interlacings == false)
		return weights;

	for (auto &&interval : parent.getIntervals(axisIndex)) {
		if (Math::Floating::is_zero(interval.isSecond)) continue;
		auto min = std::max(interval.range.min, 0.0);
		auto max = std::min(interval.range.max, 1.0);
		auto mprev = std::prev(weights.upper_bound(min));
		auto mnext = weights.lower_bound(max);

		if (mprev->first < min)
			mprev = weights.try_emplace(mprev, min, mprev->second);
		if (mnext->first > max)
			mnext = weights.try_emplace(mnext,
			    max,
			    std::prev(mnext)->second);

		while (mprev != mnext)
			mprev++->second +=
			    Math::FuzzyBool::And<double>(interval.weight,
			        interval.isSecond,
			        guides.interlacings);
	}
	return weights;
}

Gfx::Color DrawInterlacing::getCrossingInterlacingColor(
    const Gfx::Color &mainColor,
    double mainWeight,
    const Gfx::Color &otherColor,
    double otherWeight)
{
	auto color = mainColor * mainWeight + otherColor * otherWeight;

	color.alpha = 1
	            - (1 - mainColor.alpha * mainWeight)
	                  * (1 - otherColor.alpha * otherWeight);

	if (mainWeight + otherWeight > 1.0)
		color = Math::Niebloid::interpolate(color,
		    color
		        * std::max({std::abs(mainColor.red - otherColor.red),
		            std::abs(mainColor.green - otherColor.green),
		            std::abs(mainColor.blue - otherColor.blue)}),
		    mainWeight + otherWeight - 1.0);

	return color;
}

}