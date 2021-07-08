#include "drawaxes.h"

#include "chart/rendering/drawguides.h"
#include "chart/rendering/drawinterlacing.h"

#include "drawlabel.h"

using namespace Geom;
using namespace Vizzu;
using namespace Vizzu::Base;
using namespace Vizzu::Draw;
using namespace Vizzu::Diag;

drawAxes::drawAxes(const DrawingContext &context,
    const Guides &guides) :
    DrawingContext(context),
    guides(guides)
{}

void drawAxes::drawBase()
{
	if ((*style.plot.axis.interlacing.color).alpha > 0.0)
		drawInterlacing(*this, guides, false);

	auto lineBaseColor =
	    *style.plot.axis.color * (double)diagram.anyAxisSet;

	if (lineBaseColor.alpha > 0) {
		drawAxis(Diag::Scale::Type::X, lineBaseColor);
		drawAxis(Diag::Scale::Type::Y, lineBaseColor);
	}

	drawGuides(*this, guides);
}

void drawAxes::drawLabels()
{
	drawInterlacing(*this, guides, true);

	drawDiscreteLabels(true);
	drawDiscreteLabels(false);

	drawTitle(Diag::Scale::Type::X);
	drawTitle(Diag::Scale::Type::Y);
}

Geom::Line drawAxes::getAxis(Diag::Scale::Type axisIndex) const
{
	auto horizontal = axisIndex == Diag::Scale::Type::X;

	auto otherAxisIndex =
	    horizontal ? Diag::Scale::Type::Y : Diag::Scale::Type::X;

	auto offset = diagram.axises.at(otherAxisIndex).origo();

	auto direction = Point::Ident(horizontal);

	auto p0 = direction.flip() * offset;
	auto p1 = p0 + direction;

	if (offset >= 0 && offset <= 1)
		return Geom::Line(p0, p1);
	else
		return Geom::Line();
}

void drawAxes::drawAxis(Diag::Scale::Type axisIndex,
    const Gfx::Color &lineBaseColor)
{
	auto line = getAxis(axisIndex);

	if (!line.isPoint())
	{
		auto lineColor = lineBaseColor * (double)guides.at(axisIndex).axis;

		canvas.setLineColor(lineColor);
		canvas.setLineWidth(1.0);

		if (events.plot.axis.base
			->invoke(Events::OnLineDrawParam(line)))
		{
			painter.drawLine(line);
		}
	}
}

void drawAxes::drawTitle(Diag::Scale::Type axisIndex)
{
	const auto &title = diagram.axises.at(axisIndex).title;
	if (!title.empty())
	{
		auto line = getAxis(axisIndex);
		if (line.isPoint()) return;

		const auto &titleStyle = style.plot.axis.title;

		auto textBoundary = canvas.textBoundary(title);
		auto textMargin = titleStyle.toMargin(textBoundary);
		auto size = textBoundary + textMargin.getSpace();

		Geom::Point pos;
		if (axisIndex == Diag::Scale::Type::X)
		{
			auto ref = line.center();

			options.polar.get().visit([&](bool value, double weight)
			{
				auto refCopy = ref;
				if (value) refCopy.y = 1.0;

				pos = coordSys.convert(refCopy) - size.xComp() / 2
				    + Geom::Point::Y(textMargin.top);

				canvas.setTextColor(*titleStyle.color * weight);
				drawLabel(Geom::Rect(pos, size),
				    title,
				    titleStyle,
					events.plot.axis.title,
				    canvas,
				    false);
			});
		}
		else
		{
			pos = coordSys.convert(line.end) - size.yComp()
			    - size.xComp() / 2;

			drawLabel(Geom::Rect(pos, size), title, 
				style.plot.axis.title,
				events.plot.axis.title,
				canvas);
		}
	}
}

void drawAxes::drawDiscreteLabels(bool horizontal)
{
	auto &labelStyle = style.plot.axis.label;
	auto textColor = *labelStyle.color;
	if (textColor.alpha == 0.0) return;

	const auto &axises = diagram.discreteAxises;
	const auto &axis = axises.at(
	    horizontal ? Diag::Scale::Type::X : Diag::Scale::Type::Y);

	if (axis.enabled)
	{
		std::vector<std::function<void()>> tasks;

		canvas.setFont(Gfx::Font(labelStyle));

		Diag::DiscreteAxis::Values::const_iterator it;
		for (it = axis.begin();
			 it != axis.end();
			 ++it)
		{
			auto rotate = false;
			auto text = it->second.label;
			auto weight = it->second.weight;
			if (weight == 0) continue;

			auto neededSize = canvas.textBoundary(text);

			auto ident = Geom::Point::Ident(horizontal);
			auto normal = Geom::Point::Ident(!horizontal);

			auto relCenter = ident * it->second.range.middle();
			auto relMin = ident * it->second.range.getMin();
			auto relMax = ident * it->second.range.getMax();

			auto center = coordSys.convert(relCenter);
			auto min = coordSys.convert(relMin);
			auto max = coordSys.convert(relMax);
			auto margin = labelStyle.toMargin(neededSize);
			auto pad = margin.getSpace();

			auto polarRotAngle = - M_PI * (double)coordSys.getPolar();
			auto rotatedIdent = coordSys.justRotate(ident);
			auto rotatedNormal = coordSys.justRotate(
									 normal.flipY().rotated(polarRotAngle));

			auto availLength = (max-min).abs();
			auto neededLength = (rotatedIdent * neededSize).abs();

			Geom::Rect rect;
			if (!(*labelStyle.overflow == Styles::Overflow::hidden)
				|| availLength >= neededLength)
			{
				auto minWidth =
				    (rotatedIdent * Geom::Point((max - min).abs(),
				    margin.left)).abs();

				auto actWidth = std::min(minWidth, neededSize.x);
				auto halfSize = Geom::Point(actWidth/2, neededSize.y/2);
				auto halfPaddedSize = halfSize + pad/2;
				auto relCenterPos = rotatedNormal * halfPaddedSize * -1;
				auto relTextPos = halfSize * -1;
				rect = Geom::Rect(center + relCenterPos + relTextPos,
								  Geom::Size(actWidth, neededSize.y));
			}
			else
			{
				continue;
			}

			tasks.emplace_back([=]{
				if (!labelStyle.backgroundColor->isTransparent()) {
					canvas.setBrushColor(*labelStyle.backgroundColor);
					canvas.setLineColor(*labelStyle.backgroundColor);
					canvas.rectangle(rect);
				}
				canvas.setTextColor(textColor * weight);
				if (events.plot.axis.label
					->invoke(drawLabel::OnDrawParam(rect, text)))
				{
					canvas.text(rect, text, rotate ? - M_PI / 4 : 0);
				}
			});
		}
		for (auto &function : tasks) function();
	}
}
