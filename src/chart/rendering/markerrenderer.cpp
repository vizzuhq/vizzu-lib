#include "markerrenderer.h"

#include <cmath>
#include <cstddef>
#include <optional>
#include <string>
#include <utility>

#include "base/anim/interpolated.h"
#include "base/geom/line.h"
#include "base/gfx/canvas.h"
#include "base/gfx/colortransform.h"
#include "base/math/interpolation.h"
#include "base/text/smartstring.h"
#include "chart/generator/plot.h" // NOLINT(misc-include-cleaner)
#include "chart/main/events.h"
#include "chart/options/channel.h"
#include "chart/options/coordsystem.h"
#include "chart/options/shapetype.h"
#include "markers/abstractmarker.h"
#include "markers/circlemarker.h"
#include "markers/connectingmarker.h"
#include "painter/painter.h"

#include "colorbuilder.h"
#include "drawingcontext.h"
#include "orientedlabel.h"
#include "renderedchart.h"

namespace Vizzu::Draw
{

void MarkerRenderer::drawLines(Gfx::ICanvas &canvas,
    Painter &painter) const
{
	const auto &style = rootStyle.plot.marker.guides;

	if (style.color->isTransparent() || *style.lineWidth <= 0
	    || plot->anyAxisSet == false || !plot->guides.hasAnyGuides())
		return;

	canvas.setLineWidth(*style.lineWidth);

	auto origo = plot->axises.origo();

	auto xLineColor = *style.color
	                * Math::FuzzyBool::And<double>(plot->anyAxisSet,
	                    plot->guides.x.markerGuides);
	auto yLineColor = *style.color
	                * Math::FuzzyBool::And<double>(plot->anyAxisSet,
	                    plot->guides.y.markerGuides);

	for (const auto &blended : markers) {
		if (blended.marker.enabled == false
		    || blended.enabled == false)
			continue;

		if (plot->guides.x.markerGuides != false) {
			canvas.setLineColor(xLineColor);
			auto axisPoint = blended.center.xComp() + origo.yComp();
			const Geom::Line line(axisPoint, blended.center);

			auto guideElement =
			    Events::Targets::markerGuide(blended.marker, false);

			if (rootEvents.draw.plot.marker.guide->invoke(
			        Events::OnLineDrawEvent(*guideElement,
			            {line, true}))) {
				painter.drawLine(line);
				renderedChart.emplace(Draw::Line{line, true},
				    std::move(guideElement));
			}
		}
		if (plot->guides.y.markerGuides != false) {
			auto center = Geom::Point{blended.center};
			center.x = Math::interpolate(center.x,
			    1.0,
			    getOptions().coordSystem.factor(
			        Gen::CoordSystem::polar));
			canvas.setLineColor(yLineColor);
			auto axisPoint = center.yComp() + origo.xComp();
			const Geom::Line line(center, axisPoint);

			auto guideElement =
			    Events::Targets::markerGuide(blended.marker, true);

			if (rootEvents.draw.plot.marker.guide->invoke(
			        Events::OnLineDrawEvent(*guideElement,
			            {line, true}))) {
				painter.drawLine(line);
				renderedChart.emplace(Draw::Line{line, true},
				    std::move(guideElement));
			}
		}
	}
	canvas.setLineWidth(0);
}

void MarkerRenderer::drawMarkers(Gfx::ICanvas &canvas,
    Painter &painter) const
{
	for (const auto &blended : markers) {
		if (getOptions().geometry.contains(Gen::ShapeType::line)
		    && getOptions().geometry.contains(
		        Gen::ShapeType::circle)) {
			const CircleMarker circle(blended.marker,
			    coordSys,
			    getOptions(),
			    rootStyle);

			draw(canvas, painter, circle, 1, false);

			blended.marker.prevMainMarker.visit(
			    [this, &blended, &canvas, &painter](
			        ::Anim::InterpolateIndex index,
			        const auto &value)
			    {
				    draw(canvas,
				        painter,
				        ConnectingMarker{ctx(),
				            blended.marker,
				            index,
				            Gen::ShapeType::line},
				        value.weight,
				        true);
			    });
		}
		else {
			std::optional<AbstractMarker> other;
			auto drawMarker =
			    [this, &blended, &other, &canvas, &painter](
			        ::Anim::InterpolateIndex index,
			        const ::Anim::Weighted<
			            Gen::Marker::MarkerIndexPosition> &value)
			{
				if (index == ::Anim::second && !other) {
					other.emplace(
					    AbstractMarker::createInterpolated(ctx(),
					        blended.marker,
					        ::Anim::second));
				}
				const auto &blended0 =
				    index == ::Anim::second ? *other : blended;

				auto lineFactor = getOptions().geometry.factor(
				    Gen::ShapeType::line);

				draw(canvas,
				    painter,
				    blended0,
				    value.weight * (1 - lineFactor)
				        * (1 - lineFactor),
				    false);
				draw(canvas,
				    painter,
				    blended0,
				    value.weight * sqrt(lineFactor),
				    true);
			};

			auto containsConnected =
			    getOptions().geometry.contains(Gen::ShapeType::line)
			    || getOptions().geometry.contains(
			        Gen::ShapeType::area);

			auto containsSingle = getOptions().geometry.contains(
			                          Gen::ShapeType::rectangle)
			                   || getOptions().geometry.contains(
			                       Gen::ShapeType::circle);

			auto sum_weight =
			    blended.marker.prevMainMarker.combine<double>(
			        [](const Gen::Marker::MarkerIndexPosition &pos)
			        {
				        return !pos.idx.empty();
			        });
			if (containsConnected) {
				if (containsSingle) {
					auto lineIndex =
					    isConnecting(
					        getOptions()
					            .geometry.get_or_first(::Anim::first)
					            .value)
					        ? ::Anim::first
					        : ::Anim::second;

					drawMarker(lineIndex,
					    ::Anim::Weighted{blended.marker.prevMainMarker
					                         .get_or_first(lineIndex)
					                         .value,
					        sum_weight});
				}
				else
					blended.marker.prevMainMarker.visit(drawMarker);
			}
			else
				drawMarker(::Anim::first,
				    ::Anim::Weighted{blended.marker.prevMainMarker
				                         .get_or_first(::Anim::first)
				                         .value,
				        sum_weight});
		}
	}
}

void MarkerRenderer::drawLabels(Gfx::ICanvas &canvas) const
{
	auto &&axis = plot->axises.at(Gen::ChannelId::label).measure;
	auto &&keepMeasure = !axis.origMeasureName.interpolates();
	for (const auto &blended : markers) {
		if (blended.marker.enabled == false) continue;
		drawLabel(canvas,
		    blended,
		    axis.unit.get_or_first(::Anim::first).value,
		    keepMeasure,
		    ::Anim::first);
		drawLabel(canvas,
		    blended,
		    axis.unit.get_or_first(::Anim::second).value,
		    keepMeasure,
		    ::Anim::second);
	}
}

bool MarkerRenderer::shouldDrawMarkerBody(const Gen::Marker &marker)
{
	return marker.enabled != false;
}

void MarkerRenderer::draw(Gfx::ICanvas &canvas,
    Painter &painter,
    const AbstractMarker &abstractMarker,
    double factor,
    bool isLine) const
{
	if (abstractMarker.enabled == false || factor == 0) return;

	painter.setPolygonToCircleFactor(
	    isLine ? 0.0
	           : static_cast<double>(abstractMarker.morphToCircle));
	painter.setPolygonStraightFactor(
	    static_cast<double>(abstractMarker.linear));

	auto colors = getColor(abstractMarker);

	canvas.save();

	canvas.setLineWidth(*rootStyle.plot.marker.borderWidth);

	auto markerElement =
	    Events::Targets::marker(abstractMarker.marker);

	auto colorAlpha =
	    Math::FuzzyBool::And<double>(abstractMarker.enabled, factor);

	if (isLine) {
		auto line = abstractMarker.getLine();

		auto p0 = coordSys.convert(line.begin);
		auto p1 = coordSys.convert(line.end);

		colorAlpha = Math::FuzzyBool::And<double>(colorAlpha,
		    abstractMarker.connected);

		canvas.setBrushColor(colors.second * colorAlpha);
		canvas.setLineColor(colors.second * colorAlpha);

		if (rootEvents.draw.plot.marker.base->invoke(
		        Events::OnLineDrawEvent(*markerElement,
		            {Geom::Line(p0, p1), false}))) {
			painter.drawStraightLine(line,
			    abstractMarker.lineWidth,
			    static_cast<double>(abstractMarker.linear));

			renderedChart.emplace(
			    Draw::Marker{abstractMarker.marker.enabled != false,
			        abstractMarker.shapeType,
			        abstractMarker.points,
			        abstractMarker.lineWidth},
			    std::move(markerElement));
		}
	}
	else {
		auto boundary = abstractMarker.getBoundary();
		canvas.setLineColor(colors.first * colorAlpha);
		canvas.setBrushColor(colors.second * colorAlpha);

		if (rootEvents.draw.plot.marker.base->invoke(
		        Events::OnRectDrawEvent(*markerElement,
		            {boundary, true}))) {
			painter.drawPolygon(abstractMarker.points);
			renderedChart.emplace(
			    Draw::Marker{abstractMarker.marker.enabled != false,
			        abstractMarker.shapeType,
			        abstractMarker.points,
			        abstractMarker.lineWidth},
			    std::move(markerElement));
		}
	}
	canvas.setLineWidth(0);

	canvas.restore();
}

void MarkerRenderer::drawLabel(Gfx::ICanvas &canvas,
    const AbstractMarker &abstractMarker,
    const std::string &unit,
    bool keepMeasure,
    ::Anim::InterpolateIndex index) const
{
	if (abstractMarker.labelEnabled == false) return;
	const auto &marker = abstractMarker.marker;

	auto weight =
	    marker.label.interpolates() || index == ::Anim::first
	        ? marker.label.get_or_first(index).weight
	        : 0.0;
	if (weight == 0.0) return;

	auto color = getColor(abstractMarker, true).second;

	auto colorAlpha =
	    Math::FuzzyBool::And<double>(abstractMarker.labelEnabled,
	        weight);

	auto text = getLabelText(marker.label, unit, keepMeasure, index);
	if (text.empty()) return;

	const auto &labelStyle = rootStyle.plot.marker.label;

	auto labelPos = labelStyle.position->combine(
	    [this, &abstractMarker](const auto &position)
	    {
		    return abstractMarker.getLabelPos(position, coordSys);
	    });

	auto centered = labelStyle.position->factor(
	    Styles::MarkerLabel::Position::center);

	OrientedLabel{{ctx()}}.draw(canvas,
	    text.c_str(),
	    labelPos,
	    labelStyle,
	    centered,
	    Gfx::ColorTransform::OverrideColor(
	        (*labelStyle.filter)(color)*colorAlpha),
	    *rootEvents.draw.plot.marker.label,
	    Events::Targets::markerLabel(text, marker));
}

std::string MarkerRenderer::getLabelText(
    const ::Anim::Interpolated<Gen::Marker::Label> &label,
    const std::string &unit,
    bool keepMeasure,
    ::Anim::InterpolateIndex index) const
{
	const auto &labelStyle = rootStyle.plot.marker.label;
	const auto &labelValue = label.get_or_first(index).value;

	auto needsInterpolation = label.interpolates() && keepMeasure;

	std::string valueStr;
	if (labelValue.hasValue()) {
		auto value = needsInterpolation ? label.combine(
		                 [](const auto &value)
		                 {
			                 return value.value.value_or(0.0);
		                 })
		                                : labelValue.value.value();
		valueStr = Text::SmartString::fromPhysicalValue(value,
		    *labelStyle.numberFormat,
		    static_cast<size_t>(*labelStyle.maxFractionDigits),
		    *labelStyle.numberScale,
		    unit);
	}

	auto &&indexStr = labelValue.indexStr;

	typedef Styles::MarkerLabel::Format Format;
	switch (*labelStyle.format) {
	default:
	case Format::measureFirst: {
		auto text{valueStr};
		if (!indexStr.empty()) {
			if (!text.empty()) text += ", ";
			text += indexStr;
		}
		return text;
	}

	case Format::dimensionsFirst: {
		auto text{indexStr};
		if (!valueStr.empty()) {
			if (!text.empty()) text += ", ";
			text += valueStr;
		}
		return text;
	}
	}
}

std::pair<Gfx::Color, Gfx::Color> MarkerRenderer::getColor(
    const AbstractMarker &abstractMarker,
    bool label) const
{
	auto selectedColor =
	    getSelectedColor(abstractMarker.marker, label);

	auto borderAlpha = *rootStyle.plot.marker.borderOpacity;
	auto fillAlpha = *rootStyle.plot.marker.fillOpacity;

	auto fakeBgColor =
	    (*rootStyle.backgroundColor + *rootStyle.plot.backgroundColor)
	        .transparent(1.0);

	auto borderColor =
	    rootStyle.plot.marker.borderOpacityMode->combine(
	        [&](const auto &mode)
	        {
		        if (mode
		            == Styles::Marker::BorderOpacityMode::
		                premultiplied)
			        return Math::interpolate(fakeBgColor,
			            selectedColor,
			            borderAlpha);
		        return selectedColor * borderAlpha;
	        });

	auto actBorderColor = Math::interpolate(selectedColor,
	    borderColor,
	    static_cast<double>(abstractMarker.border));

	auto actItemColor = selectedColor * fillAlpha;

	return std::make_pair(actBorderColor, actItemColor);
}

Gfx::Color MarkerRenderer::getSelectedColor(const Gen::Marker &marker,
    bool label) const
{
	const ColorBuilder colorBuilder(
	    rootStyle.plot.marker.lightnessRange(),
	    *rootStyle.plot.marker.colorPalette,
	    *rootStyle.plot.marker.colorGradient);

	auto markerColor = colorBuilder.render(marker.colorBase);

	return label ? Math::interpolate(markerColor,
	           rootStyle.plot.marker.label.color->transparent(1.0),
	           rootStyle.plot.marker.label.color->alpha)
	             : markerColor;
}

MarkerRenderer MarkerRenderer::create(const DrawingContext &ctx)
{
	MarkerRenderer res{{ctx}, {}};
	for (auto &marker : res.plot->getMarkers()) {
		if (!shouldDrawMarkerBody(marker)) continue;

		res.markers.push_back(AbstractMarker::createInterpolated(ctx,
		    marker,
		    ::Anim::first));
	}
	return res;
}

}