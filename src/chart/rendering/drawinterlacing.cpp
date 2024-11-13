#include "drawinterlacing.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <utility>

#include "base/anim/interpolated.h"
#include "base/geom/point.h"
#include "base/geom/rect.h"
#include "base/gfx/colortransform.h"
#include "base/gfx/font.h"
#include "base/math/floating.h"
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
	draw(true, false);
	draw(false, false);
}

void DrawInterlacing::drawTexts() const
{
	draw(true, true);
	draw(false, true);
}

void DrawInterlacing::draw(bool horizontal, bool text) const
{
	auto axisIndex = horizontal ? Gen::AxisId::y : Gen::AxisId::x;

	auto interlacingColor =
	    *rootStyle.plot.getAxis(axisIndex).interlacing.color;

	const auto &axis = plot->axises.at(axisIndex).measure;
	auto enabled = axis.enabled.combine<double>();

	if (enabled == 0.0 || (!text && interlacingColor.alpha <= 0.0))
		return;

	auto stepI = axis.step();
	auto step = stepI.combine();

	auto &&[min, max] =
	    std::minmax(stepI.get_or_first(::Anim::first).value,
	        stepI.get_or_first(::Anim::second).value,
	        Math::Floating::less);

	auto stepHigh =
	    std::clamp(Math::Renard::R5().ceil(step), min, max);
	auto stepLow =
	    std::clamp(Math::Renard::R5().floor(step), min, max);

	if (stepHigh == step) {
		draw(axis.enabled,
		    horizontal,
		    stepHigh,
		    enabled,
		    axis.range.size(),
		    text);
	}
	else if (stepLow == step) {
		draw(axis.enabled,
		    horizontal,
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
		    horizontal,
		    stepLow,
		    lowWeight,
		    axis.range.size(),
		    text);
		draw(axis.enabled,
		    horizontal,
		    stepHigh,
		    highWeight,
		    axis.range.size(),
		    text);
	}
}

void DrawInterlacing::draw(
    const ::Anim::Interpolated<bool> &axisEnabled,
    bool horizontal,
    double stepSize,
    double weight,
    double rangeSize,
    bool text) const
{
	const auto &enabled =
	    horizontal ? plot->guides.y : plot->guides.x;

	auto axisIndex = horizontal ? Gen::AxisId::y : Gen::AxisId::x;
	const auto &axisStyle = rootStyle.plot.getAxis(axisIndex);

	auto interlacingColor =
	    *axisStyle.interlacing.color
	    * Math::FuzzyBool::And<double>(weight, enabled.interlacings);

	auto tickLength = axisStyle.ticks.length->get(
	    coordSys.getRect().size.getCoord(horizontal),
	    axisStyle.label.calculatedSize());

	auto tickColor =
	    *axisStyle.ticks.color
	    * Math::FuzzyBool::And<double>(weight, enabled.axisSticks);

	auto needTick = tickLength > 0 && !tickColor.isTransparent()
	             && *axisStyle.ticks.lineWidth > 0;

	auto textAlpha =
	    Math::FuzzyBool::And<double>(weight, enabled.labels);

	if ((!text && interlacingColor.isTransparent())
	    || (text && !needTick && textAlpha == 0.0))
		return;

	const auto &axis = plot->axises.at(axisIndex).measure;
	const auto origo = plot->axises.origo();

	auto singleLabelRange = Math::Floating::is_zero(rangeSize);

	double stripWidth{};
	if (singleLabelRange) {
		if (!text) return;
		stepSize = 1.0;
	}
	else {
		stripWidth = stepSize / rangeSize;
		if (stripWidth <= 0) return;
	}

	auto axisBottom = origo.getCoord(!horizontal) + stripWidth;

	auto iMin =
	    axisBottom > 0
	        ? std::floor(
	              -origo.getCoord(!horizontal) / (2 * stripWidth))
	              * 2
	        : std::round((axis.range.getMin() - stepSize) / stepSize);

	auto interlaceCount = 0U;
	const auto maxInterlaceCount = 1000U;
	for (auto i = static_cast<int>(iMin);
	     ++interlaceCount <= maxInterlaceCount;
	     i += 2) {
		auto bottom = axisBottom + i * stripWidth;
		if (bottom > 1.0) break;
		auto clippedBottom = bottom;
		auto top = bottom + stripWidth;
		auto clipTop = top > 1.0;
		auto clipBottom = bottom < 0.0;
		auto topUnderflow = top < 0.0;
		if (clipTop) top = 1.0;
		if (clipBottom) clippedBottom = 0.0;

		if (!topUnderflow) {
			Geom::Rect rect(Geom::Point{clippedBottom, 0.0},
			    Geom::Size{top - clippedBottom, 1.0});

			if (horizontal)
				rect =
				    Geom::Rect{rect.pos.flip(), {rect.size.flip()}};

			if (text) {
				canvas.setFont(Gfx::Font{axisStyle.label});

				if (!clipBottom) {
					auto value = (i + 1) * stepSize;
					auto tickPos = rect.bottomLeft().comp(!horizontal)
					             + origo.comp(horizontal);

					if (textAlpha > 0)
						drawDataLabel(axisEnabled,
						    horizontal,
						    tickPos,
						    value,
						    axis.unit,
						    textAlpha);

					if (needTick)
						drawSticks(tickLength,
						    tickColor,
						    horizontal,
						    tickPos);
				}
				if (singleLabelRange) break;
				if (!clipTop) {
					auto value = (i + 2) * stepSize;
					auto tickPos = rect.topRight().comp(!horizontal)
					             + origo.comp(horizontal);

					if (textAlpha > 0)
						drawDataLabel(axisEnabled,
						    horizontal,
						    tickPos,
						    value,
						    axis.unit,
						    textAlpha);

					if (needTick)
						drawSticks(tickLength,
						    tickColor,
						    horizontal,
						    tickPos);
				}
			}
			else if (!singleLabelRange) {
				canvas.save();

				canvas.setLineColor(Gfx::Color::Transparent());
				canvas.setBrushColor(interlacingColor);

				painter.setPolygonToCircleFactor(0);
				painter.setPolygonStraightFactor(0);

				if (auto &&eventTarget =
				        Events::Targets::axisInterlacing(!horizontal);
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
}

void DrawInterlacing::drawDataLabel(
    const ::Anim::Interpolated<bool> &axisEnabled,
    bool horizontal,
    const Geom::Point &tickPos,
    double value,
    const ::Anim::String &unit,
    double alpha) const
{
	auto axisIndex = horizontal ? Gen::AxisId::y : Gen::AxisId::x;
	const auto &labelStyle = rootStyle.plot.getAxis(axisIndex).label;

	auto drawLabel = OrientedLabel{{ctx()}};
	auto interpolates =
	    labelStyle.position->maxIndex() || unit.maxIndex();

	auto &&normal = Geom::Point::Ident(horizontal);
	for (auto &&index : Type::Bools{interpolates}) {
		if (labelStyle.position->interpolates()
		    && !axisEnabled.get_or_first(index).value)
			continue;
		auto &&position = labelStyle.position->get_or_first(index);

		Geom::Point refPos = tickPos;

		switch (position.value) {
			using Pos = Styles::AxisLabel::Position;
		case Pos::min_edge: refPos[horizontal ? 0 : 1] = 0.0; break;
		case Pos::max_edge: refPos[horizontal ? 0 : 1] = 1.0; break;
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
		    Events::Targets::measAxisLabel(str, !horizontal));
	}
}

void DrawInterlacing::drawSticks(double tickLength,
    const Gfx::Color &tickColor,
    bool horizontal,
    const Geom::Point &tickPos) const
{
	const auto &tickStyle =
	    rootStyle.plot
	        .getAxis(horizontal ? Gen::AxisId::y : Gen::AxisId::x)
	        .ticks;

	canvas.save();

	canvas.setLineColor(tickColor);
	canvas.setBrushColor(tickColor);

	canvas.setLineWidth(*tickStyle.lineWidth);

	auto tickLine = tickStyle.position->combine(
	    [tickLine = coordSys
	                    .convertDirectionAt({tickPos,
	                        tickPos
	                            + (horizontal ? Geom::Point::X(-1)
	                                          : Geom::Point::Y(-1))})
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

	if (auto &&eventTarget = Events::Targets::axisTick(!horizontal);
	    rootEvents.draw.plot.axis.tick->invoke(
	        Events::OnLineDrawEvent(*eventTarget,
	            {tickLine, false}))) {
		canvas.line(tickLine);
		renderedChart.emplace(Draw::Line{tickLine, false},
		    std::move(eventTarget));
	}

	canvas.restore();
}

}