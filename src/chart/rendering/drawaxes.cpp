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

	auto offset = diagram.axises.other(axisIndex).origo();

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

Geom::Point drawAxes::getTitleBasePos(Diag::Scale::Type axisIndex) const
{
	const auto &titleStyle = style.plot.axis.title;

	auto orthogonal = titleStyle.position->combine<double>([&](auto position){
		typedef Styles::AxisTitle::Position Pos;
		switch (position) {
			default:
			case Pos::min_edge: return 0.0;
			case Pos::max_edge: return 1.0;
			case Pos::axis: return diagram.axises.other(axisIndex).origo();
		}
	});

	auto parallel = titleStyle.vposition->combine<double>([&](auto position){
		typedef Styles::AxisTitle::VPosition Pos;
		switch (position) {
			default:
			case Pos::end: return 1.0;
			case Pos::middle: return 0.5;
			case Pos::begin: return 0.0;
		}
	});

	return axisIndex == Diag::Scale::Type::X 
		? Geom::Point(parallel, orthogonal)
		: Geom::Point(orthogonal, parallel);
}

Geom::Point drawAxes::getTitleOffset(Diag::Scale::Type axisIndex) const
{
	const auto &titleStyle = style.plot.axis.title;

	auto vertical = titleStyle.orientation
		->factor(Styles::AxisTitle::Orientation::vertical);

	auto orthogonal = titleStyle.side->combine<double>([&](auto side){
		typedef Styles::AxisTitle::Side Side;
		switch (side) {
			default:
			case Side::negative: return -1.0;
			case Side::positive: return 0.0;
			case Side::upon: return -0.5;
		}
	});

	auto parallel = titleStyle.vside->combine<double>([&](auto side){
		typedef Styles::AxisTitle::VSide Side;
		switch (side) {
			default:
			case Side::negative: return -1.0;
			case Side::positive: return 0.0;
			case Side::upon: return -0.5;
		}
	});

	return axisIndex == Diag::Scale::Type::X 
		? Geom::Point(parallel, orthogonal + vertical)
		: Geom::Point(orthogonal, parallel + vertical);
}

void drawAxes::drawTitle(Diag::Scale::Type axisIndex)
{
	const auto &title = diagram.axises.at(axisIndex).title;
	title.visit([&](const auto &title)
	{
		if (!title.value.empty())
		{
			const auto &titleStyle = style.plot.axis.title;

			canvas.setFont(Gfx::Font(titleStyle));
			auto textBoundary = canvas.textBoundary(title.value);
			auto textMargin = titleStyle.toMargin(textBoundary);
			auto size = textBoundary + textMargin.getSpace();

			auto base = getTitleBasePos(axisIndex);
			auto offset = getTitleOffset(axisIndex);

			auto orientedSize = titleStyle.orientation->combine<Geom::Size>(
			[&](auto orientation){
				return orientation == Styles::AxisTitle::Orientation::vertical
					? size.flip() : size;
			});

			Geom::Point pos = coordSys.convert(base) + orientedSize * offset;

			auto angle = -M_PI / 2.0 * titleStyle.orientation
				->factor(Styles::AxisTitle::Orientation::vertical);

			canvas.pushTransform(Geom::AffineTransform(pos, 1.0, angle));

			canvas.setTextColor(*titleStyle.color * title.weight);

			drawLabel(Geom::Rect(Geom::Point(), size),
				title.value,
				titleStyle,
				events.plot.axis.title,
				canvas,
				false);

			canvas.popTransform();
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
