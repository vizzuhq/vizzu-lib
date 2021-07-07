#include "drawinterlacing.h"

#include "base/math/renard.h"
#include "base/text/smartstring.h"

using namespace Geom;
using namespace Vizzu;
using namespace Vizzu::Base;
using namespace Vizzu::Draw;
using namespace Vizzu::Diag;

drawInterlacing::drawInterlacing(const DrawingContext &context,
    const Guides &guides,
    bool text) :
    DrawingContext(context),
	guides(guides)
{
	draw(true, text);
	draw(false, text);
}

void drawInterlacing::draw(bool horizontal, bool text)
{
	const auto &axis = diagram.axises.at(
		horizontal ? Diag::Scale::Type::Y : Diag::Scale::Type::X);

	auto stepHigh = Math::Renard::R5().ceil(axis.step);

	if (stepHigh == axis.step) {
		draw(horizontal, stepHigh, 1, axis.range.size(), text);
	}
	else
	{
		auto stepLow = Math::Renard::R5().floor(axis.step);

		auto highWeight = Math::Range(stepLow, stepHigh)
				.rescale(axis.step);

		auto lowWeight = 1.0 - highWeight;

		draw(horizontal, stepLow, lowWeight, axis.range.size(), text);
		draw(horizontal, stepHigh, highWeight, axis.range.size(), text);
	}
}

void drawInterlacing::draw(bool horizontal,
							 double stepSize,
							 double weight,
							 double rangeSize, bool text)
{
	auto &enabled = horizontal ? guides.x : guides.y;

	const auto &axis = diagram.axises.at(
	    horizontal ? Diag::Scale::Type::Y : Diag::Scale::Type::X);

	if ((double)(enabled.stripes || enabled.axisSticks) > 0)
	{
		auto stripeIntesity = weight * (double)enabled.stripes;
		auto stripeColor = *style.plot.axis.interlacing.color
		                   * stripeIntesity;

		auto stickIntensity = weight * (double)enabled.axisSticks;

		auto textAlpha =
		    weight * (double)(enabled.stripes || enabled.axisSticks);
		auto textColor = *style.plot.axis.label.color * textAlpha;

		if (text) {
			canvas.setTextColor(textColor);
			canvas.setFont(Gfx::Font(style.plot.axis.label));
		}
		else
		{
			canvas.setLineColor(Gfx::Color::Transparent());
			canvas.setBrushColor(stripeColor);
		}

		if (rangeSize <= 0) return;

		double stripWidth = stepSize / rangeSize;

		auto axisBottom = axis.origo() + stripWidth;

		int iMin = axisBottom > 0
				   ? -std::trunc(axisBottom/(2 * stripWidth))
				   : 0;

		for (int i = iMin; true; i++)
		{
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

			if (!topUnderflow)
			{
				std::array<Geom::Point, 4> points =
				{	P(clippedBottom, 0.0), P(clippedBottom, 1.0), P(top, 1.0), P(top, 0.0) };

				if (horizontal) for(auto &p: points) p = p.flip();

				if (text)
				{
					if(!clipBottom)
					{
						auto value = (i * 2 + 1) * stepSize;
						auto stickPos = coordSys.convert(points[0]);

						if (textColor.alpha > 0)
							drawDataLabel(horizontal, stickPos, value, axis.unit);

						if (stickIntensity > 0)
							drawSticks(stickIntensity, horizontal, stickPos);
					}
					if(!clipTop)
					{
						auto value = (i * 2 + 2) * stepSize;
						auto stickPos = coordSys.convert(points[3]);

						if (textColor.alpha > 0)
							drawDataLabel(horizontal, stickPos, value, axis.unit);

						if (stickIntensity > 0)
							drawSticks(stickIntensity, horizontal, stickPos);
					}
				}
				else
				{
					painter.setPolygonToCircleFactor(0);
					painter.setPolygonStraightFactor(0);
					if(events.plot.axis.interlacing->invoke())
						painter.drawPolygon(points);
				}
			}
		}
	}
}

void drawInterlacing::drawDataLabel(bool horizontal,
    const Geom::Point &stickPos,
    double value,
    const std::string &unit)
{
	auto &labelStyle = style.plot.axis.label;

	auto str = Text::SmartString::fromNumber(value,
	    *labelStyle.numberFormat);

	if (*labelStyle.numberFormat != Text::NumberFormat::prefixed)
		str += " ";

	str += unit;

	auto neededSize = canvas.textBoundary(str);

	auto padding = (GUI::Margin)labelStyle;

	auto relCenterPos = coordSys.justRotate(Geom::Point::Ident(horizontal))
						* ((neededSize + padding.getSpace()) / 2).flipX();

	auto relTextPos = neededSize * -1 / 2;

	auto textPos = stickPos + relCenterPos + relTextPos;

	if (!labelStyle.backgroundColor->isTransparent())
	{
		canvas.setBrushColor(*labelStyle.backgroundColor);
		canvas.setLineColor(*labelStyle.backgroundColor);
		canvas.rectangle(Geom::Rect(textPos, neededSize));
	}
	if(events.plot.axis.label->invoke())
		canvas.text(Geom::Rect(textPos, neededSize), str, 0);
}

void drawInterlacing::drawSticks(double stickIntensity,
    bool horizontal,
    const Geom::Point &stickPos)
{
	const auto &tickStyle = style.plot.axis.ticks;

	if (tickStyle.color->isTransparent()
		|| *tickStyle.length == 0
		|| *tickStyle.lineWidth == 0)
		return;

	auto stickColor = *tickStyle.color * stickIntensity;

	canvas.setLineColor(stickColor);
	canvas.setBrushColor(stickColor);

	auto dP = horizontal ? Geom::Point::X(-1) : Geom::Point::Y(-1);
	dP = coordSys.convertAt(stickPos, dP);
	dP = dP.normalized() * *tickStyle.length;

	if (*tickStyle.lineWidth > 0)
		canvas.setLineWidth(*tickStyle.lineWidth);

	auto tickLine = tickStyle.position->combine<Geom::Line>(
		[&](const auto &position)
		{
			switch (position)
			{
			default:
			case Styles::Tick::Position::outside:
				return Geom::Line(stickPos, stickPos + dP);

			case Styles::Tick::Position::inside:
				return Geom::Line(stickPos -dP, stickPos);

			case Styles::Tick::Position::center:
				return Geom::Line(stickPos - dP * 0.5, stickPos + dP * 0.5);
			}
		});

	if(events.plot.axis.tick->invoke())
		canvas.line(tickLine);
	if (*tickStyle.lineWidth > 1) canvas.setLineWidth(0);
}
