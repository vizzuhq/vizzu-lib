#include "drawinterlacing.h"

#include "base/math/renard.h"
#include "base/text/smartstring.h"
#include "chart/rendering/drawlabel.h"
#include "chart/rendering/draworientedlabel.h"

using namespace Geom;
using namespace Vizzu;
using namespace Vizzu::Base;
using namespace Vizzu::Draw;
using namespace Vizzu::Diag;

drawInterlacing::drawInterlacing(const DrawingContext &context,
    bool text) :
    DrawingContext(context)
{
	draw(true, text);
	draw(false, text);
}

void drawInterlacing::draw(bool horizontal, bool text)
{
	auto axisIndex = horizontal ? Diag::ChannelId::y : Diag::ChannelId::x;

	auto interlacingColor =
	    *style.plot.getAxis(axisIndex).interlacing.color;

	if (!text && interlacingColor.alpha <= 0.0) return;

	const auto &axis = diagram.axises.at(axisIndex);

	if (!axis.range.isReal()) return;

	auto enabled = axis.enabled.calculate<double>();

	auto step = axis.step.calculate();

	auto stepHigh = Math::Renard::R5().ceil(step);
	stepHigh = std::min(axis.step.max(),
	    std::max(stepHigh, axis.step.min()));

	auto stepLow = Math::Renard::R5().floor(step);
	stepLow =
	    std::min(axis.step.max(), std::max(stepLow, axis.step.min()));

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
		    Math::Range(stepLow, stepHigh).rescale(step) * enabled;

		auto lowWeight = (1.0 - highWeight) * enabled;

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

void drawInterlacing::draw(
    const ::Anim::Interpolated<bool> &axisEnabled,
    bool horizontal,
    double stepSize,
    double weight,
    double rangeSize,
    bool text)
{
	auto &enabled = horizontal ? diagram.guides.y : diagram.guides.x;

	auto axisIndex = horizontal ? Diag::ChannelId::y : Diag::ChannelId::x;

	auto &axisStyle = style.plot.getAxis(axisIndex);

	const auto &axis = diagram.axises.at(axisIndex);

	const auto origo = diagram.axises.origo();

	if ((double)(enabled.stripes || enabled.axisSticks
	             || enabled.labels)
	    > 0) {
		auto stripeIntesity = weight * (double)enabled.stripes;
		auto stripeColor =
		    *axisStyle.interlacing.color * stripeIntesity;

		auto stickIntensity = weight * (double)enabled.axisSticks;

		auto textAlpha = weight * (double)enabled.labels;
		auto textColor = *axisStyle.label.color * textAlpha;

		if (text) {
			canvas.setTextColor(textColor);
			canvas.setFont(Gfx::Font(axisStyle.label));
		}
		else {
			canvas.setLineColor(Gfx::Color::Transparent());
			canvas.setBrushColor(stripeColor);
		}

		if (rangeSize <= 0) return;

		double stripWidth = stepSize / rangeSize;

		auto axisBottom = axis.origo() + stripWidth;

		int iMin = axisBottom > 0
		             ? std::floor(-axis.origo() / (2 * stripWidth))
		             : 0;

		if (stripWidth <= 0) return;
		auto interlaceCount = 0u;
		const auto maxInterlaceCount = 1000u;
		for (int i = iMin; true; i++) {
			interlaceCount++;
			if (interlaceCount > maxInterlaceCount) break;
			auto bottom = axisBottom + i * 2 * stripWidth;
			if (bottom >= 1.0) break;
			auto clippedBottom = bottom;
			auto top = bottom + stripWidth;
			auto clipTop = top > 1.0;
			auto clipBottom = bottom < 0.0;
			auto topUnderflow = top <= 0.0;
			if (clipTop) top = 1.0;
			if (clipBottom) clippedBottom = 0.0;

			using P = Geom::Point;

			if (!topUnderflow) {
				std::array<Geom::Point, 4> points = {
				    P(clippedBottom, 0.0),
				    P(clippedBottom, 1.0),
				    P(top, 1.0),
				    P(top, 0.0)};

				if (horizontal)
					for (auto &p : points) p = p.flip();

				if (text) {
					if (!clipBottom) {
						auto value = (i * 2 + 1) * stepSize;
						auto stickPos = points[0].comp(!horizontal)
						              + origo.comp(horizontal);

						if (textColor.alpha > 0)
							drawDataLabel(axisEnabled,
							    horizontal,
							    stickPos,
							    value,
							    axis.unit,
							    textColor);

						if (stickIntensity > 0)
							drawSticks(stickIntensity,
							    horizontal,
							    stickPos);
					}
					if (!clipTop) {
						auto value = (i * 2 + 2) * stepSize;
						auto stickPos = points[3].comp(!horizontal)
						              + origo.comp(horizontal);

						if (textColor.alpha > 0)
							drawDataLabel(axisEnabled,
							    horizontal,
							    stickPos,
							    value,
							    axis.unit,
							    textColor);

						if (stickIntensity > 0)
							drawSticks(stickIntensity,
							    horizontal,
							    stickPos);
					}
				}
				else {
					painter.setPolygonToCircleFactor(0);
					painter.setPolygonStraightFactor(0);
					auto boundary = Geom::Rect::Boundary(points);
					auto p0 = coordSys.convert(boundary.bottomLeft());
					auto p1 = coordSys.convert(boundary.topRight());
					auto rect = Geom::Rect(p0, p1 - p0).positive();

					const char *element =
					    horizontal ? "plot.yAxis.interlacing"
					               : "plot.xAxis.interlacing";

					if (events.plot.axis.interlacing->invoke(
					        Events::OnRectDrawParam(element, rect))) {
						painter.drawPolygon(points);
					}
				}
			}
		}
	}
}

void drawInterlacing::drawDataLabel(
    const ::Anim::Interpolated<bool> &axisEnabled,
    bool horizontal,
    const Geom::Point &stickPos,
    double value,
    const std::string &unit,
    const Gfx::Color &textColor)
{
	const char *element =
	    horizontal ? "plot.yAxis.label" : "plot.xAxis.label";
	auto axisIndex = horizontal ? Diag::ChannelId::y : Diag::ChannelId::x;
	auto &labelStyle = style.plot.getAxis(axisIndex).label;

	auto str = Text::SmartString::fromNumber(value,
	    *labelStyle.numberFormat,
	    *labelStyle.maxFractionDigits,
	    *labelStyle.numberScale);

	if (!unit.empty()) {
		if (*labelStyle.numberFormat != Text::NumberFormat::prefixed)
			str += " ";

		str += unit;
	}

	auto normal = Geom::Point::Ident(horizontal);

	typedef Styles::AxisLabel::Position Pos;
	labelStyle.position->visit(
	    [&](int index, const auto &position)
	    {
		    if (labelStyle.position->interpolates()
		        && !axisEnabled.get(index).value)
			    return;

		    Geom::Point refPos = stickPos;

		    if (position.value == Pos::min_edge)
			    refPos[horizontal ? 0 : 1] = 0.0;

		    else if (position.value == Pos::max_edge)
			    refPos[horizontal ? 0 : 1] = 1.0;

		    double under =
		        labelStyle.position->interpolates()
		            ? labelStyle.side->get(index).value
		                  == Styles::AxisLabel::Side::negative
		            : labelStyle.side->factor(
		                Styles::AxisLabel::Side::negative);

		    auto sign = 1 - 2 * under;

		    auto posDir = coordSys.convertDirectionAt(
		        Geom::Line(refPos, refPos + normal));

		    posDir = posDir.extend(sign);

		    drawOrientedLabel(*this,
		        str,
		        posDir,
		        labelStyle,
		        events.plot.axis.label,
		        std::move(Events::Events::OnTextDrawParam(element)),
		        0,
		        textColor * position.weight,
		        *labelStyle.backgroundColor);
	    });
}

void drawInterlacing::drawSticks(double stickIntensity,
    bool horizontal,
    const Geom::Point &stickPos)
{
	const char *element =
	    horizontal ? "plot.yAxis.tick" : "plot.xAxis.tick";
	auto axisIndex = horizontal ? Diag::ChannelId::y : Diag::ChannelId::x;
	auto &axisStyle = style.plot.getAxis(axisIndex);
	const auto &tickStyle = axisStyle.ticks;

	auto tickLength = tickStyle.length->get(
	    coordSys.getRect().size.getCoord(horizontal),
	    axisStyle.label.calculatedSize());

	if (tickStyle.color->isTransparent() || tickLength == 0
	    || *tickStyle.lineWidth == 0)
		return;

	auto stickColor = *tickStyle.color * stickIntensity;

	canvas.setLineColor(stickColor);
	canvas.setBrushColor(stickColor);

	auto direction =
	    horizontal ? Geom::Point::X(-1) : Geom::Point::Y(-1);

	auto tickLine = coordSys.convertDirectionAt(
	    Geom::Line(stickPos, stickPos + direction));

	tickLine = tickLine.segment(0, tickLength);

	if (*tickStyle.lineWidth > 0)
		canvas.setLineWidth(*tickStyle.lineWidth);

	typedef Styles::Tick::Position Pos;
	tickLine = tickStyle.position->combine<Geom::Line>(
	    [&](int, const auto &position)
	    {
		    switch (position) {
		    default:
		    case Pos::outside: return tickLine;
		    case Pos::inside: return tickLine.segment(-1, 0);
		    case Pos::center: return tickLine.segment(-0.5, 0.5);
		    }
	    });

	if (events.plot.axis.tick->invoke(
	        Events::OnLineDrawParam(element, tickLine))) {
		canvas.line(tickLine);
	}
	if (*tickStyle.lineWidth > 1) canvas.setLineWidth(0);
}
