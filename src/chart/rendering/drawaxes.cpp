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

drawAxes::drawAxes(const DrawingContext &context) :
    DrawingContext(context)
{}

void drawAxes::drawBase()
{
	drawInterlacing(*this, false);

	drawAxis(Diag::ScaleId::x);
	drawAxis(Diag::ScaleId::y);

	drawGuides(*this);
}

void drawAxes::drawLabels()
{
	drawInterlacing(*this, true);

	drawDiscreteLabels(true);
	drawDiscreteLabels(false);

	drawTitle(Diag::ScaleId::x);
	drawTitle(Diag::ScaleId::y);
}

Geom::Line drawAxes::getAxis(Diag::ScaleId axisIndex) const
{
	auto horizontal = axisIndex == Diag::ScaleId::x;

	auto offset = diagram.axises.other(axisIndex).origo();

	auto direction = Point::Ident(horizontal);

	auto p0 = direction.flip() * offset;
	auto p1 = p0 + direction;

	if (offset >= 0 && offset <= 1)
		return Geom::Line(p0, p1);
	else
		return Geom::Line();
}

void drawAxes::drawAxis(Diag::ScaleId axisIndex)
{
	const char *element = axisIndex == Diag::ScaleId::x ? "plot.xAxis" : "plot.yAxis";

	auto lineBaseColor =
	    *style.plot.getAxis(axisIndex).color * (double)diagram.anyAxisSet;

	if (lineBaseColor.alpha <= 0) return;

	auto line = getAxis(axisIndex);

	if (!line.isPoint())
	{
		auto lineColor = lineBaseColor * (double)diagram.guides.at(axisIndex).axis;

		canvas.setLineColor(lineColor);
		canvas.setLineWidth(1.0);

		if (events.plot.axis.base
			->invoke(Events::OnLineDrawParam(element, line)))
		{
			painter.drawLine(line);
		}
	}
}

Geom::Point drawAxes::getTitleBasePos(Diag::ScaleId axisIndex) const
{
	const auto &titleStyle = style.plot.getAxis(axisIndex).title;

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

	return axisIndex == Diag::ScaleId::x 
		? Geom::Point(parallel, orthogonal)
		: Geom::Point(orthogonal, parallel);
}

Geom::Point drawAxes::getTitleOffset(Diag::ScaleId axisIndex) const
{
	const auto &titleStyle = style.plot.getAxis(axisIndex).title;

	auto vertical = titleStyle.orientation
		->factor(Styles::AxisTitle::Orientation::vertical);

	auto orthogonal = titleStyle.side->combine<double>([&](auto side){
		typedef Styles::AxisTitle::Side Side;
		switch (side) {
			default:
			case Side::positive: return -1.0;
			case Side::negative: return 0.0;
			case Side::upon: return -0.5;
		}
	});

	auto parallel = titleStyle.vside->combine<double>([&](auto side){
		typedef Styles::AxisTitle::VSide Side;
		switch (side) {
			default:
			case Side::positive: return -1.0;
			case Side::negative: return 0.0;
			case Side::upon: return -0.5;
		}
	});

	return axisIndex == Diag::ScaleId::x 
		? Geom::Point(parallel, orthogonal + vertical)
		: Geom::Point(orthogonal, parallel + vertical);
}

void drawAxes::drawTitle(Diag::ScaleId axisIndex)
{
	const auto &title = diagram.axises.at(axisIndex).title;
	const char *element = axisIndex == Diag::ScaleId::x 
		? "plot.xAxis.title" : "plot.yAxis.title";

	title.visit([&](const auto &title)
	{
		if (!title.value.empty())
		{
			const auto &titleStyle = style.plot.getAxis(axisIndex).title;

			Gfx::Font font(titleStyle);
			canvas.setFont(font);
			auto textBoundary = canvas.textBoundary(title.value);
			auto textMargin = titleStyle.toMargin(textBoundary, font.size);
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

			canvas.save();
			canvas.transform(Geom::AffineTransform(pos, 1.0, -angle));

			canvas.setTextColor(*titleStyle.color * title.weight);

			Events::Events::OnTextDrawParam param(element);
			drawLabel(Geom::Rect(Geom::Point(), size),
				title.value,
				titleStyle,
				events.plot.axis.title,
				std::move(param),
				canvas,
				false);

			canvas.restore();
		}
	});
}

void drawAxes::drawDiscreteLabels(bool horizontal)
{
	auto axisIndex = horizontal ? Diag::ScaleId::x : Diag::ScaleId::y;

	const auto &labelStyle = style.plot.getAxis(axisIndex).label;

	auto textColor = *labelStyle.color;
	if (textColor.alpha == 0.0) return;

	auto origo = diagram.axises.origo();
	const auto &axises = diagram.discreteAxises;
	const auto &axis = axises.at(axisIndex);

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
	const char *element = horizontal ? "plot.xAxis.label" : "plot.yAxis.label";
	auto &enabled = horizontal ? diagram.guides.x : diagram.guides.y;
	auto axisIndex = horizontal ? Diag::ScaleId::x : Diag::ScaleId::y;
	const auto &labelStyle = style.plot.getAxis(axisIndex).label;
	auto textColor = *labelStyle.color;

	auto text = it->second.label;
	auto weight = it->second.weight * (double)enabled.labels;
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

		auto sign = 1 - 2 * under;

		auto posDir = coordSys.convertDirectionAt(
			Geom::Line(relCenter, relCenter + normal));

		posDir = posDir.extend(sign);

		drawOrientedLabel(*this, text, posDir, labelStyle, 
			events.plot.axis.label, std::move(Events::Events::OnTextDrawParam(element)),
			0, textColor * weight * position.weight, 
			*labelStyle.backgroundColor);
	});
}
