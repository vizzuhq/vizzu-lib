#include "drawaxes.h"

#include "chart/rendering/drawguides.h"
#include "chart/rendering/drawinterlacing.h"
#include "chart/rendering/draworientedlabel.h"

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
	title.visit([&](const auto &title)
	{
		if (!title.value.empty())
		{
			auto line = getAxis(axisIndex);
			if (line.isPoint()) return;

			const auto &titleStyle = style.plot.axis.title;

			auto textBoundary = canvas.textBoundary(title.value);
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

					canvas.setTextColor(*titleStyle.color 
						* weight * title.weight);

					drawLabel(Geom::Rect(pos, size),
						title.value,
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

				canvas.setTextColor(*titleStyle.color * title.weight);

				drawLabel(Geom::Rect(pos, size), title.value, 
					style.plot.axis.title,
					events.plot.axis.title,
					canvas,
					false);
			}
		}
	});
}

void drawAxes::drawDiscreteLabels(bool horizontal)
{
	auto &labelStyle = style.plot.axis.label;
	auto textColor = *labelStyle.color;
	if (textColor.alpha == 0.0) return;

	auto origo = diagram.axises.origo();
	const auto &axises = diagram.discreteAxises;
	const auto &axis = axises.at(
	    horizontal ? Diag::Scale::Type::X : Diag::Scale::Type::Y);

	if (axis.enabled)
	{
		canvas.setFont(Gfx::Font(labelStyle));

		Diag::DiscreteAxis::Values::const_iterator it;
		for (it = axis.begin();
			 it != axis.end();
			 ++it)
		{
			drawDiscreteLabel(horizontal, origo, it);
		}
	}
}

void drawAxes::drawDiscreteLabel(bool horizontal,
	const Geom::Point &origo,
	Diag::DiscreteAxis::Values::const_iterator it)
{
	auto &labelStyle = style.plot.axis.label;
	auto textColor = *labelStyle.color;

	auto text = it->second.label;
	auto weight = it->second.weight;
	if (weight == 0) return;

	auto ident = Geom::Point::Ident(horizontal);
	auto normal = Geom::Point::Ident(!horizontal);

	typedef Styles::AxisLabel::Position Pos;
	labelStyle.position->visit(
	[&](const auto &position)
	{
		Geom::Point refPos;

		switch(position.value) {
			case Pos::max_edge: refPos = Geom::Point::Ident(!horizontal); break;
			case Pos::axis: refPos = origo.comp(!horizontal); break;
			default:
			case Pos::min_edge: refPos = Geom::Point(); break;
		}

		auto relCenter = refPos + ident * it->second.range.middle();

		double under = labelStyle.side->factor
			(Styles::AxisLabel::Side::negative);

		auto direction = normal * (1 - 2 * under);

		auto posDir = coordSys.convertDirectionAt(
			Geom::Line(relCenter, relCenter + direction));

		drawOrientedLabel(*this, text, posDir, labelStyle, 0, 
			textColor * weight * position.weight, 
			*labelStyle.backgroundColor);
	});
}
