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
	auto axisIndex =
	    horizontal ? Gen::ChannelId::y : Gen::ChannelId::x;

	auto interlacingColor =
	    *rootStyle.plot.getAxis(axisIndex).interlacing.color;

	if (!text && interlacingColor.alpha <= 0.0) return;

	const auto &axis = plot->axises.at(axisIndex).measure;

	if (!axis.range.isReal()) return;

	auto enabled = axis.enabled.calculate<double>();

	auto step = axis.step.calculate();

	auto stepHigh = std::clamp(Math::Renard::R5().ceil(step),
	    axis.step.min(),
	    axis.step.max());
	auto stepLow = std::clamp(Math::Renard::R5().floor(step),
	    axis.step.min(),
	    axis.step.max());

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
		    Math::Range<double>::Raw(stepLow, stepHigh).rescale(step);

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

	auto axisIndex =
	    horizontal ? Gen::ChannelId::y : Gen::ChannelId::x;

	const auto &axisStyle = rootStyle.plot.getAxis(axisIndex);

	const auto &axis = plot->axises.at(axisIndex).measure;

	const auto origo = plot->axises.origo();

	if ((enabled.interlacings || enabled.axisSticks || enabled.labels)
	    != false) {
		auto interlaceIntensity = Math::FuzzyBool::And<double>(weight,
		    enabled.interlacings);
		auto interlaceColor =
		    *axisStyle.interlacing.color * interlaceIntensity;

		auto tickIntensity =
		    Math::FuzzyBool::And<double>(weight, enabled.axisSticks);

		auto textAlpha =
		    Math::FuzzyBool::And<double>(weight, enabled.labels);

		if (std::signbit(rangeSize) != std::signbit(stepSize)
		    || Math::Floating::is_zero(rangeSize))
			return;

		auto stripWidth = stepSize / rangeSize;

		auto axisBottom = axis.origo() + stripWidth;

		auto iMin =
		    axisBottom > 0 ? static_cast<int>(
		        std::floor(-axis.origo() / (2 * stripWidth)))
		                   : static_cast<int>(
		                       (axis.range.getMin() - stepSize) / 2);

		if (stripWidth <= 0) return;
		auto interlaceCount = 0U;
		const auto maxInterlaceCount = 1000U;
		for (int i = iMin; ++interlaceCount <= maxInterlaceCount;
		     ++i) {
			auto bottom = axisBottom + i * 2 * stripWidth;
			if (bottom >= 1.0) break;
			auto clippedBottom = bottom;
			auto top = bottom + stripWidth;
			auto clipTop = top > 1.0;
			auto clipBottom = bottom < 0.0;
			auto topUnderflow = top <= 0.0;
			if (clipTop) top = 1.0;
			if (clipBottom) clippedBottom = 0.0;

			if (!topUnderflow) {
				Geom::Rect rect(Geom::Point{clippedBottom, 0.0},
				    Geom::Size{top - clippedBottom, 1.0});

				if (horizontal)
					rect = Geom::Rect{rect.pos.flip(),
					    {rect.size.flip()}};

				if (text) {
					canvas.setFont(Gfx::Font{axisStyle.label});

					if (!clipBottom) {
						auto value = (i * 2 + 1) * stepSize;
						auto tickPos =
						    rect.bottomLeft().comp(!horizontal)
						    + origo.comp(horizontal);

						if (textAlpha > 0)
							drawDataLabel(axisEnabled,
							    horizontal,
							    tickPos,
							    value,
							    axis.unit,
							    textAlpha);

						if (tickIntensity > 0)
							drawSticks(tickIntensity,
							    horizontal,
							    tickPos);
					}
					if (!clipTop) {
						auto value = (i * 2 + 2) * stepSize;
						auto tickPos =
						    rect.topRight().comp(!horizontal)
						    + origo.comp(horizontal);

						if (textAlpha > 0)
							drawDataLabel(axisEnabled,
							    horizontal,
							    tickPos,
							    value,
							    axis.unit,
							    textAlpha);

						if (tickIntensity > 0)
							drawSticks(tickIntensity,
							    horizontal,
							    tickPos);
					}
				}
				else {
					canvas.save();

					canvas.setLineColor(Gfx::Color::Transparent());
					canvas.setBrushColor(interlaceColor);

					painter.setPolygonToCircleFactor(0);
					painter.setPolygonStraightFactor(0);

					auto eventTarget =
					    Events::Targets::axisInterlacing(!horizontal);

					if (rootEvents.draw.plot.axis.interlacing->invoke(
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
}

void DrawInterlacing::drawDataLabel(
    const ::Anim::Interpolated<bool> &axisEnabled,
    bool horizontal,
    const Geom::Point &tickPos,
    double value,
    const ::Anim::String &unit,
    double alpha) const
{
	auto axisIndex =
	    horizontal ? Gen::ChannelId::y : Gen::ChannelId::x;
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

void DrawInterlacing::drawSticks(double tickIntensity,
    bool horizontal,
    const Geom::Point &tickPos) const
{
	auto axisIndex =
	    horizontal ? Gen::ChannelId::y : Gen::ChannelId::x;
	const auto &axisStyle = rootStyle.plot.getAxis(axisIndex);
	const auto &tickStyle = axisStyle.ticks;

	auto tickLength = tickStyle.length->get(
	    coordSys.getRect().size.getCoord(horizontal),
	    axisStyle.label.calculatedSize());

	if (tickStyle.color->isTransparent() || tickLength == 0
	    || *tickStyle.lineWidth == 0)
		return;

	auto tickColor = *tickStyle.color * tickIntensity;

	canvas.save();

	canvas.setLineColor(tickColor);
	canvas.setBrushColor(tickColor);

	auto direction =
	    horizontal ? Geom::Point::X(-1) : Geom::Point::Y(-1);

	auto tickLine =
	    coordSys.convertDirectionAt({tickPos, tickPos + direction});

	tickLine = tickLine.segment(0, tickLength);

	if (*tickStyle.lineWidth > 0)
		canvas.setLineWidth(*tickStyle.lineWidth);

	typedef Styles::Tick::Position Pos;
	tickLine = tickStyle.position->combine(
	    [&tickLine](const auto &position)
	    {
		    switch (position) {
		    default:
		    case Pos::outside: return tickLine;
		    case Pos::inside: return tickLine.segment(-1, 0);
		    case Pos::center: return tickLine.segment(-0.5, 0.5);
		    }
	    });

	auto eventTarget = Events::Targets::axisTick(!horizontal);

	if (rootEvents.draw.plot.axis.tick->invoke(
	        Events::OnLineDrawEvent(*eventTarget,
	            {tickLine, false}))) {
		canvas.line(tickLine);
		renderedChart.emplace(Draw::Line{tickLine, false},
		    std::move(eventTarget));
	}
	if (*tickStyle.lineWidth > 1) canvas.setLineWidth(0);

	canvas.restore();
}

}