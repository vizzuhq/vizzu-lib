#include "drawaxes.h"

#include "base/type/booliter.h"
#include "chart/generator/plot.h"
#include "chart/rendering/drawguides.h"
#include "chart/rendering/orientedlabel.h"

#include "drawlabel.h"

namespace Vizzu::Draw
{

void DrawAxes::drawGeometries() const
{
	interlacing.drawGeometries();

	drawAxis(Gen::ChannelId::x);
	drawAxis(Gen::ChannelId::y);

	DrawGuides{{ctx()}, canvas, painter}.draw();
}

void DrawAxes::drawLabels() const
{
	interlacing.drawTexts();

	drawDimensionLabels(true);
	drawDimensionLabels(false);

	drawTitle(Gen::ChannelId::x);
	drawTitle(Gen::ChannelId::y);
}

Geom::Line DrawAxes::getAxis(Gen::ChannelId axisIndex) const
{
	auto horizontal = axisIndex == Gen::ChannelId::x;

	auto offset = plot->measureAxises.other(axisIndex).origo();

	auto direction = Geom::Point::Ident(horizontal);

	auto p0 = direction.flip() * offset;
	auto p1 = p0 + direction;

	if (offset >= 0 && offset <= 1) return {p0, p1};
	return {};
}

void DrawAxes::drawAxis(Gen::ChannelId axisIndex) const
{
	auto eventTarget =
	    Events::Targets::axis(axisIndex == Gen::ChannelId::x);

	auto lineBaseColor = *rootStyle.plot.getAxis(axisIndex).color
	                   * static_cast<double>(plot->anyAxisSet);

	if (lineBaseColor.alpha <= 0) return;

	auto line = getAxis(axisIndex);

	if (!line.isPoint()) {
		auto lineColor =
		    lineBaseColor
		    * static_cast<double>(plot->guides.at(axisIndex).axis);

		canvas.save();

		canvas.setLineColor(lineColor);
		canvas.setLineWidth(1.0);

		if (rootEvents.draw.plot.axis.base->invoke(
		        Events::OnLineDrawEvent(*eventTarget,
		            {line, true}))) {
			painter.drawLine(line);
			renderedChart.emplace(Draw::Line{line, true},
			    std::move(eventTarget));
		}

		canvas.restore();
	}
}

Geom::Point DrawAxes::getTitleBasePos(Gen::ChannelId axisIndex,
    ::Anim::InterpolateIndex index) const
{
	typedef Styles::AxisTitle::Position Pos;
	typedef Styles::AxisTitle::VPosition VPos;

	const auto &titleStyle = rootStyle.plot.getAxis(axisIndex).title;

	double orthogonal{0.0};

	switch (titleStyle.position->get(index).value) {
	default:
	case Pos::min_edge: break;
	case Pos::max_edge: orthogonal = 1.0; break;
	case Pos::axis:
		orthogonal = plot->measureAxises.other(axisIndex).origo();
		break;
	}

	double parallel{0.0};

	switch (titleStyle.vposition->get(index).value) {
	default:
	case VPos::end: parallel = 1.0; break;
	case VPos::middle: parallel = 0.5; break;
	case VPos::begin: break;
	}

	return axisIndex == Gen::ChannelId::x
	         ? Geom::Point{parallel, orthogonal}
	         : Geom::Point{orthogonal, parallel};
}

Geom::Point DrawAxes::getTitleOffset(Gen::ChannelId axisIndex,
    ::Anim::InterpolateIndex index,
    bool fades) const
{
	const auto &titleStyle = rootStyle.plot.getAxis(axisIndex).title;

	auto calcSide = [](const auto &side)
	{
		typedef Styles::AxisTitle::Side Side;
		switch (side) {
		default:
		case Side::positive: return 1.0;
		case Side::negative: return -1.0;
		case Side::upon: return 0.0;
		}
	};

	auto orthogonal = fades
	                    ? calcSide(titleStyle.side->get(index).value)
	                    : titleStyle.side->combine<double>(calcSide);

	auto calcVSide = [](const auto &side)
	{
		typedef Styles::AxisTitle::VSide Side;
		switch (side) {
		default:
		case Side::positive: return 1.0;
		case Side::negative: return -1.0;
		case Side::upon: return 0.0;
		}
	};

	auto parallel = fades
	                  ? calcVSide(titleStyle.vside->get(index).value)
	                  : titleStyle.vside->combine<double>(calcVSide);

	return axisIndex == Gen::ChannelId::x
	         ? Geom::Point{parallel, -orthogonal}
	         : Geom::Point{orthogonal, -parallel};
}

void DrawAxes::drawTitle(Gen::ChannelId axisIndex) const
{
	const auto &titleString = plot->commonAxises.at(axisIndex).title;

	const auto &titleStyle = rootStyle.plot.getAxis(axisIndex).title;

	auto fades = titleStyle.position->maxIndex()
	          || titleStyle.vposition->maxIndex()
	          || titleString.maxIndex();

	for (auto &&index : Type::Bools{fades}) {
		auto title = titleString.get(index);
		if (title.value.empty()) continue;

		auto weight = title.weight
		            * titleStyle.position->get(index).weight
		            * titleStyle.vposition->get(index).weight;

		const Gfx::Font font(titleStyle);
		canvas.setFont(font);
		auto size = titleStyle.extendSize(
		    Gfx::ICanvas::textBoundary(font, title.value),
		    font.size);

		auto relCenter = getTitleBasePos(axisIndex, index);

		auto normal = Geom::Point::Ident(true);

		auto offset = getTitleOffset(axisIndex,
		    index,
		    fades == ::Anim::secondIfExists);

		auto posDir = coordSys.convertDirectionAt(
		    {relCenter, relCenter + normal});

		auto posAngle = posDir.getDirection().angle();

		canvas.save();

		Geom::AffineTransform transform(posDir.begin, 1.0, -posAngle);

		auto calcOrientation =
		    [&size](const Styles::AxisTitle::Orientation &orientation)
		{
			return Geom::Size{
			    orientation
			            == Styles::AxisTitle::Orientation::vertical
			        ? size.flip()
			        : size};
		};

		auto angle =
		    -M_PI / 2.0
		    * (fades == ::Anim::secondIfExists
		            ? titleStyle.orientation->get(index).value
		                  == Styles::AxisTitle::Orientation::vertical
		            : titleStyle.orientation->factor<double>(
		                Styles::AxisTitle::Orientation::vertical));

		auto orientedSize =
		    fades == ::Anim::secondIfExists
		        ? calcOrientation(
		            titleStyle.orientation->get(index).value)
		        : titleStyle.orientation->combine<Geom::Size>(
		            calcOrientation);

		auto center = offset * (orientedSize / 2.0);

		transform = transform
		          * Geom::AffineTransform(center, 1.0, angle)
		          * Geom::AffineTransform((orientedSize / -2.0));

		auto realAngle = Geom::Angle(-posAngle + angle).rad();
		auto upsideDown =
		    realAngle > M_PI / 2.0 && realAngle < 3 * M_PI / 2.0;

		DrawLabel{{ctx()}}.draw(canvas,
		    Geom::TransformedRect{transform, Geom::Size{size}},
		    title.value,
		    titleStyle,
		    *rootEvents.draw.plot.axis.title,
		    Events::Targets::axisTitle(title.value,
		        axisIndex == Gen::ChannelId::x),
		    {.alpha = weight, .flip = upsideDown});

		canvas.restore();
	}
}

void DrawAxes::drawDimensionLabels(bool horizontal) const
{
	auto axisIndex =
	    horizontal ? Gen::ChannelId::x : Gen::ChannelId::y;

	const auto &labelStyle = rootStyle.plot.getAxis(axisIndex).label;

	auto textColor = *labelStyle.color;
	if (textColor.alpha == 0.0) return;

	auto origo = plot->measureAxises.origo();
	const auto &axises = plot->dimensionAxises;
	const auto &axis = axises.at(axisIndex);

	if (axis.enabled) {
		canvas.setFont(Gfx::Font{labelStyle});

		for (auto it = axis.begin(); it != axis.end(); ++it) {
			drawDimensionLabel(horizontal, origo, it, axis.category);
		}
	}
}

void DrawAxes::drawDimensionLabel(bool horizontal,
    const Geom::Point &origo,
    Gen::DimensionAxis::Values::const_iterator it,
    const std::string_view &category) const
{
	const auto &enabled =
	    horizontal ? plot->guides.x : plot->guides.y;

	auto weight =
	    it->second.weight * static_cast<double>(enabled.labels);
	if (weight == 0) return;

	auto axisIndex =
	    horizontal ? Gen::ChannelId::x : Gen::ChannelId::y;
	const auto &labelStyle = rootStyle.plot.getAxis(axisIndex).label;

	auto drawLabel = OrientedLabel{{ctx()}};
	labelStyle.position->visit(
	    [this,
	        &drawLabel,
	        &labelStyle,
	        &it,
	        &horizontal,
	        &origo,
	        ident = Geom::Point::Ident(horizontal),
	        normal = Geom::Point::Ident(!horizontal),
	        &text = it->second.label,
	        &categoryVal = it->second.categoryValue,
	        &weight,
	        &category](::Anim::InterpolateIndex index,
	        const auto &position)
	    {
		    if (labelStyle.position->interpolates()
		        && !it->second.presentAt(index))
			    return;

		    Geom::Point refPos;

		    switch (position.value) {
			    using Pos = Styles::AxisLabel::Position;
		    case Pos::max_edge: refPos = normal; break;
		    case Pos::axis: refPos = origo.comp(!horizontal); break;
		    default:
		    case Pos::min_edge: refPos = Geom::Point(); break;
		    }

		    auto relCenter =
		        refPos + ident * it->second.range.middle();

		    auto under = labelStyle.position->interpolates()
		                   ? labelStyle.side->get(index).value
		                         == Styles::AxisLabel::Side::negative
		                   : labelStyle.side->factor<double>(
		                       Styles::AxisLabel::Side::negative);

		    auto sign = 1 - 2 * under;

		    auto posDir = coordSys.convertDirectionAt(
		        {relCenter, relCenter + normal});

		    posDir = posDir.extend(sign);

		    auto draw = [&](const ::Anim::Weighted<std::string> &str,
		                    double plusWeight = 1.0)
		    {
			    drawLabel.draw(canvas,
			        str.value,
			        posDir,
			        labelStyle,
			        0,
			        weight * str.weight * plusWeight,
			        1.0,
			        *rootEvents.draw.plot.axis.label,
			        Events::Targets::dimAxisLabel(category,
			            categoryVal,
			            categoryVal,
			            horizontal));
		    };

		    if (labelStyle.position->interpolates()
		        && text.interpolates())
			    draw(text.get(index), position.weight);
		    if (!labelStyle.position->interpolates()
		        && !text.interpolates())
			    draw(text.get(::Anim::first));
		    else if (labelStyle.position->interpolates())
			    draw(text.get(::Anim::first), position.weight);
		    else if (text.interpolates()) {
			    draw(text.get(::Anim::first));
			    draw(text.get(::Anim::secondIfExists));
		    }
	    });
}

}