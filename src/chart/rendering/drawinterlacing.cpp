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

void DrawInterlacing::drawGeometries() const
{
	draw(Gen::AxisId::y, false);
	draw(Gen::AxisId::x, false);
}

void DrawInterlacing::drawTexts() const
{
	draw(Gen::AxisId::y, true);
	draw(Gen::AxisId::x, true);
}

void DrawInterlacing::draw(Gen::AxisId axisIndex, bool text) const
{
	const auto &axis = plot->axises.at(axisIndex).measure;
	auto enabled = axis.enabled.combine<double>();

	if (enabled == 0.0) return;

	auto step = axis.step.combine();

	auto &&[min, max] =
	    std::minmax(axis.step.get_or_first(::Anim::first).value,
	        axis.step.get_or_first(::Anim::second).value,
	        Math::Floating::less);

	auto stepHigh =
	    std::clamp(Math::Renard::R5().ceil(step), min, max);
	auto stepLow =
	    std::clamp(Math::Renard::R5().floor(step), min, max);

	if (Math::Floating::is_zero(axis.range.size()))
		step = stepHigh = stepLow = 1.0;

	if (stepHigh == step) {
		draw(axis.enabled,
		    axisIndex,
		    stepHigh,
		    enabled,
		    axis.range.size(),
		    text);
	}
	else if (stepLow == step) {
		draw(axis.enabled,
		    axisIndex,
		    stepLow,
		    enabled,
		    axis.range.size(),
		    text);
	}
	else {
		auto highWeight =
		    Math::Range<>::Raw(stepLow, stepHigh).rescale(step);

		auto lowWeight = (1.0 - highWeight) * enabled;
		highWeight *= enabled;

		draw(axis.enabled,
		    axisIndex,
		    stepLow,
		    lowWeight,
		    axis.range.size(),
		    text);
		draw(axis.enabled,
		    axisIndex,
		    stepHigh,
		    highWeight,
		    axis.range.size(),
		    text);
	}
}

void DrawInterlacing::draw(
    const ::Anim::Interpolated<bool> &axisEnabled,
    Gen::AxisId axisIndex,
    double stepSize,
    double weight,
    double rangeSize,
    bool text) const
{
	auto orientation = !+axisIndex;
	const auto &guides = plot->guides.at(axisIndex);
	const auto &axisStyle = rootStyle.plot.getAxis(axisIndex);
	const auto &axis = plot->axises.at(axisIndex).measure;

	auto interlacingColor =
	    *axisStyle.interlacing.color
	    * Math::FuzzyBool::And<double>(weight, guides.interlacings);

	auto tickLength = axisStyle.ticks.length->get(
	    coordSys.getRect().size.getCoord(orientation),
	    axisStyle.label.calculatedSize());

	auto tickColor =
	    *axisStyle.ticks.color
	    * Math::FuzzyBool::And<double>(weight, guides.axisSticks);

	auto needTick = tickLength > 0 && !tickColor.isTransparent()
	             && *axisStyle.ticks.lineWidth > 0;

	auto textAlpha =
	    Math::FuzzyBool::And<double>(weight, guides.labels);

	if ((!text && interlacingColor.isTransparent())
	    || (text && !needTick && textAlpha == 0.0))
		return;

	auto singleLabelRange = Math::Floating::is_zero(rangeSize);
	if (singleLabelRange && !text) return;

	double stripWidth{};
	if (!singleLabelRange) {
		stripWidth = stepSize / rangeSize;
		if (stripWidth <= 0) return;
	}

	const auto origo = plot->axises.origo();
	auto axisBottom = origo.getCoord(!orientation) + stripWidth;

	auto iMin = static_cast<int>(
	    axisBottom > 0 ? std::floor(-origo.getCoord(!orientation)
	                                / (2 * stripWidth))
	                         * 2
	                   : std::round((axis.range.getMin() - stepSize)
	                                / stepSize));

	if (axisBottom + iMin * stripWidth + stripWidth < 0.0)
		iMin += 2
		      * static_cast<int>(std::ceil(
		          -(axisBottom + stripWidth) / (2 * stripWidth)));

	if (!text) {
		painter.setPolygonToCircleFactor(0);
		painter.setPolygonStraightFactor(0);
	}

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

	for (auto &&[i, bottom] :
	    std::views::iota(0) | std::views::transform(Transform)
	        | std::views::take_while(Predicate)) {
		auto clippedBottom =
		    std::max(bottom, 0.0, Math::Floating::less);
		auto clippedSize =
		    std::min(bottom + stripWidth, 1.0, Math::Floating::less)
		    - clippedBottom;
		auto rect = Geom::Rect{
		    Geom::Point::Coord(orientation, 0.0, clippedBottom),
		    {Geom::Size::Coord(orientation, 1.0, clippedSize)}};

		if (text) {
			if (auto tickPos = rect.bottomLeft().comp(!orientation)
			                 + origo.comp(orientation);
			    bottom >= 0.0) {
				if (textAlpha > 0)
					drawDataLabel(axisEnabled,
					    axisIndex,
					    tickPos,
					    (i + 1) * stepSize,
					    axis.unit,
					    textAlpha);

				if (needTick)
					drawSticks(tickLength,
					    tickColor,
					    axisIndex,
					    tickPos);
			}
			if (singleLabelRange) break;
			if (auto tickPos = rect.topRight().comp(!orientation)
			                 + origo.comp(orientation);
			    bottom + stripWidth <= 1.0) {
				if (textAlpha > 0)
					drawDataLabel(axisEnabled,
					    axisIndex,
					    tickPos,
					    (i + 2) * stepSize,
					    axis.unit,
					    textAlpha);

				if (needTick)
					drawSticks(tickLength,
					    tickColor,
					    axisIndex,
					    tickPos);
			}
		}
		else {
			canvas.save();
			canvas.setLineColor(Gfx::Color::Transparent());
			canvas.setBrushColor(interlacingColor);
			if (auto &&eventTarget =
			        Events::Targets::axisInterlacing(axisIndex);
			    rootEvents.draw.plot.axis.interlacing->invoke(
			        Events::OnRectDrawEvent(*eventTarget,
			            {rect, true}))) {
				painter.drawPolygon(rect.points());
				renderedChart.emplace(Draw::Rect{rect, true},
				    std::move(eventTarget));
			}
			canvas.restore();
		}
	}
}

void DrawInterlacing::drawDataLabel(
    const ::Anim::Interpolated<bool> &axisEnabled,
    Gen::AxisId axisIndex,
    const Geom::Point &tickPos,
    double value,
    const ::Anim::String &unit,
    double alpha) const
{
	auto orientation = !+axisIndex;
	const auto &labelStyle = rootStyle.plot.getAxis(axisIndex).label;

	auto drawLabel = OrientedLabel{{ctx()}};
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
		    coordSys.convertDirectionAt({refPos, refPos + normal})
		        .extend(1 - 2 * under);

		auto &&wUnit = unit.get_or_first(index);
		auto str = Text::SmartString::fromPhysicalValue(value,
		    *labelStyle.numberFormat,
		    static_cast<size_t>(*labelStyle.maxFractionDigits),
		    *labelStyle.numberScale,
		    wUnit.value);
		drawLabel.draw(canvas,
		    str,
		    posDir,
		    labelStyle,
		    0,
		    Gfx::ColorTransform::Opacity(Math::FuzzyBool::And(alpha,
		        position.weight,
		        wUnit.weight)),
		    *rootEvents.draw.plot.axis.label,
		    Events::Targets::measAxisLabel(str, axisIndex));
	}
}

void DrawInterlacing::drawSticks(double tickLength,
    const Gfx::Color &tickColor,
    Gen::AxisId axisIndex,
    const Geom::Point &tickPos) const
{
	const auto &tickStyle = rootStyle.plot.getAxis(axisIndex).ticks;

	canvas.save();

	canvas.setLineColor(tickColor);
	canvas.setBrushColor(tickColor);

	canvas.setLineWidth(*tickStyle.lineWidth);

	auto tickLine = tickStyle.position->combine(
	    [tickLine =
	            coordSys
	                .convertDirectionAt({tickPos,
	                    tickPos
	                        + Geom::Point::Coord(!+axisIndex, -1.0)})
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
	    rootEvents.draw.plot.axis.tick->invoke(
	        Events::OnLineDrawEvent(*eventTarget,
	            {tickLine, false}))) {
		canvas.line(tickLine);
		renderedChart.emplace(Line{tickLine, false},
		    std::move(eventTarget));
	}

	canvas.restore();
}

}