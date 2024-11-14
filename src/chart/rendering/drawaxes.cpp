#include "drawaxes.h"

#include <numbers>
#include <string>
#include <utility>

#include "base/anim/interpolated.h"
#include "base/geom/affinetransform.h"
#include "base/geom/line.h"
#include "base/geom/orientation.h"
#include "base/geom/point.h"
#include "base/geom/transformedrect.h"
#include "base/gfx/colortransform.h"
#include "base/gfx/font.h"
#include "base/math/fuzzybool.h"
#include "base/type/booliter.h"
#include "chart/generator/plot.h" // NOLINT(misc-include-cleaner)
#include "chart/main/events.h"
#include "chart/main/style.h"
#include "chart/options/channel.h"
#include "dataframe/old/types.h"

#include "drawguides.h"
#include "drawlabel.h"
#include "orientedlabel.h"
#include "renderedchart.h"

namespace Vizzu::Draw
{

void DrawAxes::drawGeometries() const
{
	interlacing.drawGeometries();

	drawAxis(Gen::AxisId::x);
	drawAxis(Gen::AxisId::y);

	DrawGuides{{ctx()}, canvas, painter}.draw();
}

void DrawAxes::drawLabels() const
{
	interlacing.drawTexts();

	drawDimensionLabels(Gen::AxisId::x);
	drawDimensionLabels(Gen::AxisId::y);

	drawTitle(Gen::AxisId::x);
	drawTitle(Gen::AxisId::y);
}

Geom::Line DrawAxes::getAxis(Gen::AxisId axisIndex) const
{
	auto offset = plot->axises.other(axisIndex).measure.origo();

	auto direction = Geom::Point::Ident(+axisIndex);

	auto p0 = direction.flip() * offset;
	auto p1 = p0 + direction;

	if (offset >= 0 && offset <= 1) return {p0, p1};
	return {};
}

void DrawAxes::drawAxis(Gen::AxisId axisIndex) const
{
	if (auto line = getAxis(axisIndex); !line.isPoint()) {
		auto lineColor =
		    *rootStyle.plot.getAxis(axisIndex).color
		    * static_cast<double>(plot->guides.at(axisIndex).axis);

		if (lineColor.isTransparent()) return;

		canvas.save();

		canvas.setLineColor(lineColor);
		canvas.setLineWidth(1.0);

		if (auto &&eventTarget = Events::Targets::axis(axisIndex);

		    rootEvents.draw.plot.axis.base->invoke(
		        Events::OnLineDrawEvent(*eventTarget,
		            {line, true}))) {
			painter.drawLine(line);
			renderedChart.emplace(Draw::Line{line, true},
			    std::move(eventTarget));
		}

		canvas.restore();
	}
}

Geom::Point DrawAxes::getTitleBasePos(Gen::AxisId axisIndex,
    ::Anim::InterpolateIndex index) const
{
	typedef Styles::AxisTitle::Position Pos;
	typedef Styles::AxisTitle::VPosition VPos;

	const auto &titleStyle = rootStyle.plot.getAxis(axisIndex).title;

	double orthogonal{0.0};

	switch (titleStyle.position->get_or_first(index).value) {
	default:
	case Pos::min_edge: break;
	case Pos::max_edge: orthogonal = 1.0; break;
	case Pos::axis:
		orthogonal = plot->axises.other(axisIndex).measure.origo();
		break;
	}

	double parallel{0.0};

	switch (titleStyle.vposition->get_or_first(index).value) {
	default:
	case VPos::end: parallel = 1.0; break;
	case VPos::middle: parallel = 0.5; break;
	case VPos::begin: break;
	}

	return axisIndex == Gen::AxisId::x
	         ? Geom::Point{parallel, orthogonal}
	         : Geom::Point{orthogonal, parallel};
}

Geom::Point DrawAxes::getTitleOffset(Gen::AxisId axisIndex,
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

	auto orthogonal =
	    fades ? calcSide(titleStyle.side->get_or_first(index).value)
	          : titleStyle.side->combine(calcSide);

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

	auto parallel =
	    fades ? calcVSide(titleStyle.vside->get_or_first(index).value)
	          : titleStyle.vside->combine(calcVSide);

	return axisIndex == Gen::AxisId::x
	         ? Geom::Point{parallel, -orthogonal}
	         : Geom::Point{orthogonal, -parallel};
}

void DrawAxes::drawTitle(Gen::AxisId axisIndex) const
{
	const auto &titleString = plot->axises.at(axisIndex).title;

	const auto &titleStyle = rootStyle.plot.getAxis(axisIndex).title;

	auto fades = titleStyle.position->maxIndex()
	          || titleStyle.vposition->maxIndex()
	          || titleString.maxIndex();

	for (auto &&index : Type::Bools{fades}) {
		auto title = titleString.get_or_first(index);
		if (title.value.empty()) continue;

		auto weight = Math::FuzzyBool::And(title.weight,
		    titleStyle.position->get_or_first(index).weight,
		    titleStyle.vposition->get_or_first(index).weight);

		const Gfx::Font font(titleStyle);
		canvas.setFont(font);
		auto size = titleStyle.extendSize(
		    Gfx::ICanvas::textBoundary(font, title.value),
		    font.size);

		auto relCenter = getTitleBasePos(axisIndex, index);

		auto normal = Geom::Point::X(1.0);

		auto offset =
		    getTitleOffset(axisIndex, index, fades == ::Anim::second);

		auto posDir = coordSys.convertDirectionAt(
		    {relCenter, relCenter + normal});

		auto posAngle = posDir.getDirection().angle();

		canvas.save();

		Geom::AffineTransform transform(posDir.begin, 1.0, -posAngle);

		auto calcOrientation =
		    [&size](const Styles::AxisTitle::Orientation &orientation)
		{
			return Geom::Size{
			    isHorizontal(orientation) ? size : size.flip()};
		};

		auto angle =
		    -std::numbers::pi / 2.0
		    * (fades == ::Anim::second
		            ? !isHorizontal(
		                titleStyle.orientation->get_or_first(index)
		                    .value)
		            : titleStyle.orientation->factor(
		                Styles::AxisTitle::Orientation::vertical));

		auto orientedSize =
		    fades == ::Anim::second
		        ? calcOrientation(
		            titleStyle.orientation->get_or_first(index).value)
		        : titleStyle.orientation->combine(calcOrientation);

		auto center = offset * (orientedSize / 2.0);

		transform = transform
		          * Geom::AffineTransform(center, 1.0, angle)
		          * Geom::AffineTransform((orientedSize / -2.0));

		auto realAngle = Geom::Angle(-posAngle + angle).rad();
		auto upsideDown = realAngle > std::numbers::pi / 2.0
		               && realAngle < 3 * std::numbers::pi / 2.0;

		DrawLabel{{ctx()}}.draw(canvas,
		    Geom::TransformedRect{transform, Geom::Size{size}},
		    title.value,
		    titleStyle,
		    *rootEvents.draw.plot.axis.title,
		    Events::Targets::axisTitle(title.value, axisIndex),
		    {.colorTransform = Gfx::ColorTransform::Opacity(weight),
		        .flip = upsideDown});

		canvas.restore();
	}
}

void DrawAxes::drawDimensionLabels(Gen::AxisId axisIndex) const
{
	const auto &labelStyle = rootStyle.plot.getAxis(axisIndex).label;

	auto textColor = *labelStyle.color;
	if (textColor.isTransparent()) return;

	auto origo = plot->axises.origo();
	const auto &axises = plot->axises;
	const auto &axis = axises.at(axisIndex).dimension;

	if (!axis.empty()) {
		canvas.setFont(Gfx::Font{labelStyle});
		const auto &enabled = plot->guides.at(axisIndex);

		for (const auto &[slice, item] : axis.getValues())
			drawDimensionLabel(axisIndex,
			    origo,
			    item,
			    slice,
			    Math::FuzzyBool::And<double>(item.weight(axis.factor),
			        enabled.labels));
	}
}

void DrawAxes::drawDimensionLabel(Gen::AxisId axisIndex,
    const Geom::Point &origo,
    const Gen::DimensionAxis::Item &item,
    const Data::SliceIndex &index,
    double weight) const
{
	if (weight == 0) return;
	auto orientation = +axisIndex;

	const auto &labelStyle = rootStyle.plot.getAxis(axisIndex).label;

	auto drawLabel = OrientedLabel{{ctx()}};
	labelStyle.position->visit(
	    [this,
	        &axisIndex,
	        &drawLabel,
	        &labelStyle,
	        &item,
	        &orientation,
	        &origo,
	        ident = Geom::Point::Ident(orientation),
	        normal = Geom::Point::Ident(!orientation),
	        &text = item.label,
	        &weight,
	        &sindex = index](::Anim::InterpolateIndex index,
	        const auto &position)
	    {
		    if (labelStyle.position->interpolates()
		        && !item.presentAt(index))
			    return;

		    Geom::Point refPos;

		    switch (position.value) {
			    using Pos = Styles::AxisLabel::Position;
		    case Pos::max_edge: refPos = normal; break;
		    case Pos::axis: refPos = origo.comp(!orientation); break;
		    default:
		    case Pos::min_edge: refPos = Geom::Point(); break;
		    }

		    auto relCenter = refPos + ident * item.range.middle();

		    auto under =
		        labelStyle.position->interpolates()
		            ? labelStyle.side->get_or_first(index).value
		                  == Styles::AxisLabel::Side::negative
		            : labelStyle.side->factor(
		                Styles::AxisLabel::Side::negative);

		    auto draw =
		        [&,
		            posDir = coordSys
		                         .convertDirectionAt(
		                             {relCenter, relCenter + normal})
		                         .extend(1 - 2 * under)](
		            const ::Anim::Weighted<bool> &str,
		            double plusWeight = 1.0)
		    {
			    if (!str.value) return;
			    drawLabel.draw(canvas,
			        sindex.value,
			        posDir,
			        labelStyle,
			        0,
			        Gfx::ColorTransform::Opacity(
			            Math::FuzzyBool::And(weight,
			                str.weight,
			                plusWeight)),
			        *rootEvents.draw.plot.axis.label,
			        Events::Targets::dimAxisLabel(sindex.column,
			            sindex.value,
			            axisIndex));
		    };

		    if (text.interpolates()
		        && !labelStyle.position->interpolates()) {
			    draw(text.get_or_first(::Anim::first));
			    draw(text.get_or_first(::Anim::second));
		    }
		    else {
			    draw(text.get_or_first(index),
			        !labelStyle.position->interpolates()
			            ? 1.0
			            : position.weight);
		    }
	    });
}

}