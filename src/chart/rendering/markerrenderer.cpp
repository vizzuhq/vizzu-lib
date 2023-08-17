#include "markerrenderer.h"

#include "base/geom/angle.h"
#include "base/text/smartstring.h"
#include "chart/rendering/drawlabel.h"
#include "chart/rendering/draworientedlabel.h"
#include "chart/rendering/markers/abstractmarker.h"
#include "chart/rendering/markers/circlemarker.h"
#include "chart/rendering/markers/connectingmarker.h"
#include "chart/rendering/markers/rectanglemarker.h"

using namespace Geom;
using namespace Vizzu;
using namespace Vizzu::Base;
using namespace Vizzu::Draw;
using namespace Vizzu::Gen;

MarkerRenderer::MarkerRenderer(const Gen::Marker &marker,
    const DrawingContext &context) :
    DrawingContext(context),
    marker(marker)
{}

void MarkerRenderer::drawLines(const Styles::Guide &style,
    const Geom::Point &origo)
{
	if (static_cast<double>(marker.enabled) == 0) return;

	auto blended = AbstractMarker::createInterpolated(marker,
	    options,
	    plot.getStyle(),
	    coordSys,
	    plot.getMarkers(),
	    0);

	auto baseColor =
	    *style.color * static_cast<double>(plot.anyAxisSet);

	if (static_cast<double>(blended.enabled) > 0) {
		if (static_cast<double>(plot.guides.x.guidelines) > 0) {
			auto lineColor =
			    baseColor
			    * static_cast<double>(plot.guides.x.guidelines);
			canvas.setLineColor(lineColor);
			auto axisPoint = blended.center.xComp() + origo.yComp();
			const Geom::Line line(axisPoint, blended.center);
			if (rootEvents.plot.marker.guide->invoke(
			        Events::OnLineDrawParam("plot.marker.guide.x",
			            line,
			            marker.idx))) {
				painter.drawLine(line);
			}
		}
		if (static_cast<double>(plot.guides.y.guidelines) > 0) {
			blended.center.x = Math::interpolate(blended.center.x,
			    1.0,
			    options.coordSystem.factor<double>(
			        Gen::CoordSystem::polar));
			auto lineColor =
			    baseColor
			    * static_cast<double>(plot.guides.y.guidelines);
			canvas.setLineColor(lineColor);
			auto axisPoint = blended.center.yComp() + origo.xComp();
			const Geom::Line line(blended.center, axisPoint);
			if (rootEvents.plot.marker.guide->invoke(
			        Events::OnLineDrawParam("plot.marker.guide.y",
			            line,
			            marker.idx))) {
				painter.drawLine(line);
			}
		}
	}
}

void MarkerRenderer::draw()
{
	if (!shouldDrawMarkerBody()) return;

	if (options.geometry.contains(Gen::ShapeType::line)
	    && options.geometry.contains(Gen::ShapeType::circle)) {
		const CircleMarker circle(marker,
		    coordSys,
		    options,
		    plot.getStyle());

		draw(circle, 1, false);

		marker.prevMainMarkerIdx.visit(
		    [this](int index, auto value)
		    {
			    const ConnectingMarker line(marker,
			        coordSys,
			        options,
			        plot.getStyle(),
			        plot.getMarkers(),
			        index,
			        Gen::ShapeType::line);

			    draw(line, value.weight, true);
		    });
	}
	else {
		auto drawMarker =
		    [this](int index, ::Anim::Weighted<uint64_t> value)
		{
			auto blended0 = AbstractMarker::createInterpolated(marker,
			    options,
			    plot.getStyle(),
			    coordSys,
			    plot.getMarkers(),
			    index);

			auto lineFactor = options.geometry.factor<double>(
			    Gen::ShapeType::line);

			draw(blended0,
			    value.weight * (1 - lineFactor) * (1 - lineFactor),
			    false);
			draw(blended0, value.weight * sqrt(lineFactor), true);
		};

		auto containsConnected =
		    options.geometry.contains(Gen::ShapeType::line)
		    || options.geometry.contains(Gen::ShapeType::area);

		auto containsSingle =
		    options.geometry.contains(Gen::ShapeType::rectangle)
		    || options.geometry.contains(Gen::ShapeType::circle);

		if (containsConnected) {
			if (containsSingle) {
				auto lineIndex =
				    Gen::isConnecting(options.geometry.get(0).value)
				        ? 0
				        : 1;

				drawMarker(lineIndex, ::Anim::Weighted<uint64_t>(0));
			}
			else
				marker.prevMainMarkerIdx.visit(drawMarker);
		}
		else
			drawMarker(0, ::Anim::Weighted<uint64_t>(0));
	}
}

void MarkerRenderer::drawLabel()
{
	if (static_cast<double>(marker.enabled) == 0) return;

	auto blended = AbstractMarker::createInterpolated(marker,
	    options,
	    plot.getStyle(),
	    coordSys,
	    plot.getMarkers(),
	    0);

	drawLabel(blended, 0);
	drawLabel(blended, 1);
}

bool MarkerRenderer::shouldDrawMarkerBody()
{
	bool enabled = static_cast<double>(marker.enabled) > 0;
	if (options.geometry.factor<Math::FuzzyBool>(
	        Gen::ShapeType::area)
	    != false) {
		const auto *prev0 =
		    ConnectingMarker::getPrev(marker, plot.getMarkers(), 0);

		const auto *prev1 =
		    ConnectingMarker::getPrev(marker, plot.getMarkers(), 1);

		if (prev0) enabled |= static_cast<double>(prev0->enabled) > 0;
		if (prev1) enabled |= static_cast<double>(prev1->enabled) > 0;
	}
	return enabled;
}

void MarkerRenderer::draw(const AbstractMarker &abstractMarker,
    double factor,
    bool line)
{
	if (static_cast<double>(abstractMarker.enabled) == 0
	    || factor == 0)
		return;

	painter.setPolygonToCircleFactor(
	    line ? 0.0
	         : static_cast<double>(abstractMarker.morphToCircle));
	painter.setPolygonStraightFactor(
	    static_cast<double>(abstractMarker.linear));
	painter.setResMode(ResolutionMode::High);

	auto colors = getColor(abstractMarker, factor);

	canvas.setLineColor(colors.first);
	canvas.setLineWidth(*rootStyle.plot.marker.borderWidth);
	canvas.setBrushColor(colors.second);

	auto boundary = abstractMarker.getBoundary();

	auto p0 = coordSys.convert(boundary.bottomLeft());
	auto p1 = coordSys.convert(boundary.topRight());
	auto rect = Geom::Rect(p0, p1 - p0).positive();

	if (line) {
		auto line = abstractMarker.getLine();

		auto p0 = coordSys.convert(line.begin);
		auto p1 = coordSys.convert(line.end);

		if (rootEvents.plot.marker.base->invoke(
		        Events::OnLineDrawParam("plot.marker",
		            Geom::Line(p0, p1),
		            abstractMarker.marker.idx))) {
			painter.drawStraightLine(line,
			    abstractMarker.lineWidth,
			    static_cast<double>(abstractMarker.linear),
			    colors.second,
			    colors.second
			        * static_cast<double>(abstractMarker.connected));
		}
	}
	else {
		if (rootEvents.plot.marker.base->invoke(
		        Events::OnRectDrawParam("plot.marker",
		            rect,
		            abstractMarker.marker.idx))) {
			painter.drawPolygon(abstractMarker.points);
		}
	}
	canvas.setLineWidth(0);
}

void MarkerRenderer::drawLabel(const AbstractMarker &abstractMarker,
    size_t index)
{
	if (static_cast<double>(abstractMarker.labelEnabled) == 0) return;

	auto weight = marker.label.values[index].weight;
	if (weight == 0.0) return;

	auto color = getColor(abstractMarker, 1, true).second;

	auto text = getLabelText(index);
	if (text.empty()) return;

	const auto &labelStyle = rootStyle.plot.marker.label;

	auto labelPos = labelStyle.position->combine<Geom::Line>(
	    [this, &abstractMarker](int, const auto &position)
	    {
		    return abstractMarker.getLabelPos(position, coordSys);
	    });

	auto textColor = (*labelStyle.filter)(color)*weight;
	auto bgColor = *labelStyle.backgroundColor * weight;

	auto centered = labelStyle.position->factor<double>(
	    Styles::MarkerLabel::Position::center);

	Events::Events::OnTextDrawParam param("plot.marker.label");
	param.markerIndex = marker.idx;
	DrawOrientedLabel(*this,
	    text,
	    labelPos,
	    labelStyle,
	    rootEvents.plot.marker.label,
	    std::move(param),
	    centered,
	    textColor,
	    bgColor);
}

std::string MarkerRenderer::getLabelText(size_t index) const
{
	const auto &labelStyle = rootStyle.plot.marker.label;
	const auto &values = marker.label.values;

	auto needsInterpolation =
	    marker.label.count == 2
	    && (values[0].value.measureId == values[1].value.measureId);

	std::string valueStr;
	if (values[index].value.hasValue()) {
		auto value = needsInterpolation
		               ? marker.label.combine<double>(
		                   [&](int, const auto &value)
		                   {
			                   return value.value.value_or(0);
		                   })
		               : values[index].value.value.value();
		valueStr = Text::SmartString::fromNumber(value,
		    *labelStyle.numberFormat,
		    static_cast<size_t>(*labelStyle.maxFractionDigits),
		    *labelStyle.numberScale);

		if (!values[index].value.unit.empty()) {
			if (*labelStyle.numberFormat
			    != Text::NumberFormat::prefixed)
				valueStr += " ";

			valueStr += values[index].value.unit;
		}
	}

	auto indexStr = values[index].value.indexStr;

	typedef Styles::MarkerLabel::Format Format;
	switch (static_cast<Format>(*labelStyle.format)) {
	default:
	case Format::measureFirst: {
		auto text = valueStr;
		if (!indexStr.empty()) {
			if (!text.empty()) text += ", ";
			text += indexStr;
		}
		return text;
	}

	case Format::dimensionsFirst: {
		auto text = indexStr;
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
    double factor,
    bool label)
{
	auto selectedColor = getSelectedColor(label);

	auto borderAlpha = *rootStyle.plot.marker.borderOpacity;
	auto fillAlpha = *rootStyle.plot.marker.fillOpacity;

	auto fakeBgColor =
	    (*rootStyle.backgroundColor + *rootStyle.plot.backgroundColor)
	        .transparent(1.0);

	auto borderColor =
	    rootStyle.plot.marker.borderOpacityMode->combine<Gfx::Color>(
	        [&](int, const auto &mode)
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

	const auto &enabled =
	    label ? abstractMarker.labelEnabled : abstractMarker.enabled;
	auto alpha = static_cast<double>(enabled) * factor;

	auto finalBorderColor = actBorderColor * alpha;
	auto itemColor = selectedColor * alpha * fillAlpha;

	double highlight = 0.0;
	double anyHighlight = 0.0;
	auto markerInfo = plot.getMarkersInfo();
	for (auto &info : markerInfo) {
		auto allHighlight = 0.0;
		info.second.visit(
		    [this, &highlight, &allHighlight](int, const auto &info)
		    {
			    highlight += info.value.markerId == this->marker.idx
			                   ? 1.0
			                   : 0.0;
			    if (info.value.markerId.has_value())
				    allHighlight += info.weight;
		    });
		anyHighlight = std::max(anyHighlight, allHighlight);
	}

	auto highlightAlpha = 1 - (0.65 * anyHighlight) * (1 - highlight);
	finalBorderColor = (finalBorderColor * highlightAlpha);
	itemColor = (itemColor * highlightAlpha);

	return std::make_pair(finalBorderColor, itemColor);
}

Gfx::Color MarkerRenderer::getSelectedColor(bool label)
{
	auto orig =
	    label ? Math::interpolate(marker.color,
	        rootStyle.plot.marker.label.color->transparent(1.0),
	        rootStyle.plot.marker.label.color->alpha)
	          : marker.color;

	auto gray = orig.desaturate().lightnessScaled(0.75);
	auto interpolated = Math::interpolate(gray,
	    orig,
	    static_cast<double>(marker.selected));

	return Math::interpolate(orig,
	    interpolated,
	    static_cast<double>(plot.anySelected));
}
