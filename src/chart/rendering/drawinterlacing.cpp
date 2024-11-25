#include "drawinterlacing.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <ranges>
#include <utility>

#include "base/anim/interpolated.h"
#include "base/geom/point.h"
#include "base/geom/rect.h"
#include "base/gfx/colortransform.h"
#include "base/math/floating.h"
#include "base/math/fuzzybool.h"
#include "base/math/range.h"
#include "base/math/renard.h"
#include "base/text/smartstring.h"
#include "base/type/booliter.h"
#include "chart/generator/plot.h" // NOLINT(misc-include-cleaner)
#include "chart/main/events.h"
#include "chart/options/channel.h"

#include "orientedlabel.h"
#include "renderedchart.h"

namespace Vizzu::Draw
{

void DrawInterlacing::drawGeometries(Gen::AxisId axisIndex) const
{
	const auto &guides = parent.plot->guides.at(axisIndex);
	const auto &axisStyle = parent.rootStyle.plot.getAxis(axisIndex);
	if (axisStyle.interlacing.color->isTransparent()
	    || guides.interlacings == false)
		return;

	std::map<double, double> othWeights{{0.0, 0.0}, {1.0, 0.0}};

	const auto &othGuides = parent.plot->guides.at(!axisIndex);
	const auto &othAxisStyle =
	    parent.rootStyle.plot.getAxis(!axisIndex);
	if (!othAxisStyle.interlacing.color->isTransparent()
	    && othGuides.interlacings != false)
		for (const auto &othInterval :
		    parent.getIntervals(!axisIndex)) {
			if (Math::Floating::is_zero(othInterval.isSecond))
				continue;
			auto min = std::max(othInterval.range.getMin(), 0.0);
			auto max = std::min(othInterval.range.getMax(), 1.0);
			auto mprev = std::prev(othWeights.upper_bound(min));
			auto mnext = othWeights.lower_bound(max);

			if (mprev->first < min)
				mprev =
				    othWeights.try_emplace(mprev, min, mprev->second);
			if (mnext->first > max)
				mnext = othWeights.try_emplace(mnext,
				    max,
				    std::prev(mnext)->second);

			while (mprev != mnext)
				mprev++->second +=
				    Math::FuzzyBool::And<double>(othInterval.weight,
				        othInterval.isSecond,
				        othGuides.interlacings);
		}

	auto orientation = ++!axisIndex;

	parent.painter.setPolygonToCircleFactor(0);
	parent.painter.setPolygonStraightFactor(0);

	for (const auto &interval : parent.getIntervals(axisIndex)) {
		if (Math::Floating::is_zero(interval.isSecond)) continue;
		auto clippedBottom = std::max(interval.range.getMin(),
		    0.0,
		    Math::Floating::less);
		auto clippedSize = std::min(interval.range.getMax(),
		                       1.0,
		                       Math::Floating::less)
		                 - clippedBottom;

		auto rect = [&](const double &from, const double &to)
		{
			return Geom::Rect{
			    Geom::Point::Coord(orientation, from, clippedBottom),
			    {Geom::Size::Coord(orientation,
			        to - from,
			        clippedSize)}};
		};

		auto weight = Math::FuzzyBool::And<double>(interval.weight,
		    interval.isSecond,
		    guides.interlacings);
		auto interlacingColor = *axisStyle.interlacing.color * weight;

		for (auto first = othWeights.begin(),
		          next = std::next(first),
		          last = othWeights.end();
		     next != last;
		     ++next, ++first) {
			if (Math::Floating::is_zero(first->second))
				drawInterlacing(axisIndex,
				    interlacingColor,
				    rect(first->first, next->first));
			else if (axisIndex == Gen::AxisId::y) {
				auto color =
				    interlacingColor
				    + *othAxisStyle.interlacing.color * first->second;

				color.alpha =
				    1
				    - (1
				          - axisStyle.interlacing.color->alpha
				                * weight)
				          * (1
				              - othAxisStyle.interlacing.color->alpha
				                    * first->second);

				if (weight + first->second > 1.0)
					color = Math::Niebloid::interpolate(
					    color
					        * std::max({std::abs(axisStyle.interlacing
					                                 .color->red
					                             - color.red),
					            std::abs(
					                axisStyle.interlacing.color->green
					                - color.green),
					            std::abs(
					                axisStyle.interlacing.color->blue
					                - color.blue)}),
					    color,
					    2.0 - (weight + first->second));
				drawInterlacing(first->second > weight ? !axisIndex
				                                       : axisIndex,
				    color,
				    rect(first->first, next->first));
			}
		}
	}
}

void DrawInterlacing::drawTexts(Gen::AxisId axisIndex) const
{
	const auto &axis = parent.getAxis(axisIndex).measure;
	auto orientation = ++!axisIndex;
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

	for (const auto &sep : parent.getSeparators(axisIndex)) {
		auto tickPos =
		    Geom::Point::Coord(orientation, origo, sep.position);
		if (needText && sep.label)
			drawDataLabel(axis.enabled,
			    axisIndex,
			    tickPos,
			    *sep.label,
			    axis.unit,
			    Math::FuzzyBool::And<double>(sep.weight,
			        guides.labels));

		if (needTick)
			drawSticks(tickLength,
			    *axisStyle.ticks.color
			        * Math::FuzzyBool::And<double>(sep.weight,
			            guides.axisSticks),
			    axisIndex,
			    tickPos);
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
    double value,
    const ::Anim::String &unit,
    double alpha) const
{
	auto orientation = ++!axisIndex;
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

		auto &&posDir =
		    parent.coordSys
		        .convertDirectionAt({refPos, refPos + normal})
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
    const Geom::Point &tickPos) const
{
	auto &canvas = parent.canvas;
	const auto &tickStyle =
	    parent.rootStyle.plot.getAxis(axisIndex).ticks;

	canvas.save();

	canvas.setLineColor(tickColor);
	canvas.setBrushColor(tickColor);

	canvas.setLineWidth(*tickStyle.lineWidth);

	auto tickLine = tickStyle.position->combine(
	    [tickLine =
	            parent.coordSys
	                .convertDirectionAt({tickPos,
	                    tickPos
	                        + Geom::Point::Coord(++!axisIndex, -1.0)})
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

}