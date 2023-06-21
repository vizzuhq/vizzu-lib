#include "drawitem.h"

#include "base/geom/angle.h"
#include "base/text/smartstring.h"
#include "chart/rendering/drawlabel.h"
#include "chart/rendering/draworientedlabel.h"
#include "chart/rendering/items/areaitem.h"
#include "chart/rendering/items/blendeditem.h"
#include "chart/rendering/items/circleitem.h"
#include "chart/rendering/items/drawitem.h"
#include "chart/rendering/items/lineitem.h"
#include "chart/rendering/items/rectangleitem.h"

using namespace Geom;
using namespace Vizzu;
using namespace Vizzu::Base;
using namespace Vizzu::Draw;
using namespace Vizzu::Gen;

drawItem::drawItem(const Gen::Marker &marker,
    const DrawingContext &context) :
    DrawingContext(context),
    marker(marker)
{}

void drawItem::drawLines(const Styles::Guide &style,
    const Geom::Point &origo)
{
	if ((double)marker.enabled == 0) return;

	BlendedDrawItem blended(marker,
	    options,
	    plot.getStyle(),
	    coordSys,
	    plot.getMarkers(),
	    0);

	auto baseColor = *style.color * (double)plot.anyAxisSet;

	if ((double)blended.enabled > 0) {
		if ((double)plot.guides.x.guidelines > 0) {
			auto lineColor =
			    baseColor * (double)plot.guides.x.guidelines;
			canvas.setLineColor(lineColor);
			auto axisPoint = blended.center.xComp() + origo.yComp();
			Geom::Line line(axisPoint, blended.center);
			if (events.plot.marker.guide->invoke(
			        Events::OnLineDrawParam("plot.marker.guide.x",
			            line,
			            marker.idx))) {
				painter.drawLine(line);
			}
		}
		if ((double)plot.guides.y.guidelines > 0) {
			blended.center.x = Math::interpolate(blended.center.x,
			    1.0,
			    (double)options.polar);
			auto lineColor =
			    baseColor * (double)plot.guides.y.guidelines;
			canvas.setLineColor(lineColor);
			auto axisPoint = blended.center.yComp() + origo.xComp();
			Geom::Line line(blended.center, axisPoint);
			if (events.plot.marker.guide->invoke(
			        Events::OnLineDrawParam("plot.marker.guide.y",
			            line,
			            marker.idx))) {
				painter.drawLine(line);
			}
		}
	}
}

void drawItem::draw()
{
	if (!shouldDraw()) return;

	if (drawOptions.onlyEssentials() && (double)plot.anySelected
	    && (double)marker.selected == 0)
		return;

	auto lineFactor = (double)options.shapeType.factor(
	    Gen::ShapeType::line);

	auto circleFactor = (double)options.shapeType.factor(
	    Gen::ShapeType::circle);

	if (lineFactor > 0 && circleFactor) {
		CircleItem circle(marker,
		    options, plot.getStyle(),
		    coordSys);

		LineItem line(marker,
		    options,
		    plot.getStyle(),
		    coordSys,
		    plot.getMarkers(),
		    0);

		draw(circle, 1, false);
		draw(line, 1, true);
	}
	else {
		BlendedDrawItem blended0(marker,
		    options,
		    plot.getStyle(),
		    coordSys,
		    plot.getMarkers(),
		    0);
		/*
		        BlendedDrawItem blended1(marker,
		            options,
		            plot.getStyle(),
		            plot.getMarkers(),
		            1);
		*/
		draw(blended0, (1 - lineFactor) * (1 - lineFactor), false);
		draw(blended0, sqrt(lineFactor), true);
		//		draw(blended1, sqrt(lineFactor), true, false);
	}
}

void drawItem::drawLabel()
{
	if ((double)marker.enabled == 0) return;

	BlendedDrawItem blended(marker,
	    options,
	    plot.getStyle(),
	    coordSys,
	    plot.getMarkers(),
	    0);

	drawLabel(blended, 0);
	drawLabel(blended, 1);
}

bool drawItem::shouldDraw()
{
	bool enabled = (double)marker.enabled > 0;
	if ((double)options.shapeType.factor(
	        Gen::ShapeType::area)
	    > 0) {
		const auto *prev0 = ConnectingDrawItem::getPrev(marker, plot.getMarkers(),
		    0);

		const auto *prev1 = ConnectingDrawItem::getPrev(marker, plot.getMarkers(),
		    1);

		if (prev0) enabled |= (double)prev0->enabled > 0;
		if (prev1) enabled |= (double)prev1->enabled > 0;
	}
	return enabled;
}

void drawItem::draw(const DrawItem &drawItem,
    double factor,
    bool line)
{
	if ((double)drawItem.enabled == 0 || factor == 0) return;

	painter.setPolygonToCircleFactor(
	    line ? 0.0 : (double)drawItem.morphToCircle);
	painter.setPolygonStraightFactor((double)drawItem.linear);
	painter.setResMode(drawOptions.getResoultionMode());

	auto colors = getColor(drawItem, factor);

	canvas.setLineColor(colors.first);
	canvas.setLineWidth(*style.plot.marker.borderWidth);
	canvas.setBrushColor(colors.second);

	auto boundary = drawItem.getBoundary();

	auto p0 = coordSys.convert(boundary.bottomLeft());
	auto p1 = coordSys.convert(boundary.topRight());
	auto rect = Geom::Rect(p0, p1 - p0).positive();

	if (line) {
		auto line = drawItem.getLine();

		auto p0 = coordSys.convert(line.begin);
		auto p1 = coordSys.convert(line.end);

		if (events.plot.marker.base->invoke(
		        Events::OnLineDrawParam("plot.marker",
		            Geom::Line(p0, p1),
		            drawItem.marker.idx))) {
			painter.drawStraightLine(line,
			    drawItem.lineWidth,
			    colors.second,
			    colors.second * (double)drawItem.connected);
		}
	}
	else {
		if (events.plot.marker.base->invoke(
		        Events::OnRectDrawParam("plot.marker",
		            rect,
		            drawItem.marker.idx))) {
			painter.drawPolygon(drawItem.points);
		}
	}
	canvas.setLineWidth(0);
}

void drawItem::drawLabel(const DrawItem &drawItem, size_t index)
{
	if ((double)drawItem.labelEnabled == 0) return;

	auto weight = marker.label.values[index].weight;
	if (weight == 0.0) return;

	auto color = getColor(drawItem, 1, true).second;

	auto text = getLabelText(index);
	if (text.empty()) return;

	auto &labelStyle = style.plot.marker.label;

	auto labelPos = labelStyle.position->combine<Geom::Line>(
	    [&](int, const auto &position)
	    {
		    return drawItem.getLabelPos(position, coordSys);
	    });

	auto textColor = (*labelStyle.filter)(color)*weight;
	auto bgColor = *labelStyle.backgroundColor * weight;

	auto centered = labelStyle.position->factor(
	    Styles::MarkerLabel::Position::center);

	Events::Events::OnTextDrawParam param("plot.marker.label");
	param.markerIndex = marker.idx;
	drawOrientedLabel(*this,
	    text,
	    labelPos,
	    labelStyle,
	    events.plot.marker.label,
	    std::move(param),
	    centered,
	    textColor,
	    bgColor);
}

std::string drawItem::getLabelText(size_t index) const
{
	auto &labelStyle = style.plot.marker.label;
	auto &values = marker.label.values;

	auto needsInterpolation = marker.label.count == 2
	                       && (values[0].value.measureId == values[1].value.measureId);

	auto value = needsInterpolation ? marker.label.combine<double>(
	                 [&](int, const auto &value)
	                 {
		                 return value.value;
	                 })
	                                : values[index].value.value;

	std::string valueStr;
	if (values[index].value.hasValue()) {
		valueStr = Text::SmartString::fromNumber(value,
		    *labelStyle.numberFormat,
		    *labelStyle.maxFractionDigits,
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
	switch ((Format)*labelStyle.format) {
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

std::pair<Gfx::Color, Gfx::Color> drawItem::getColor(
    const DrawItem &drawItem,
    double factor,
    bool label)
{
	auto selectedColor = getSelectedColor();

	auto borderAlpha = *style.plot.marker.borderOpacity;
	auto fillAlpha = *style.plot.marker.fillOpacity;

	auto fakeBgColor =
	    (*style.backgroundColor + *style.plot.backgroundColor)
	        .transparent(1.0);

	auto borderColor = style.plot.marker.borderOpacityMode->combine<
	    Gfx::Color>(
	    [&](int, const auto &mode)
	    {
		    if (mode
		        == Styles::Marker::BorderOpacityMode::premultiplied)
			    return Math::interpolate(fakeBgColor,
			        selectedColor,
			        borderAlpha);
		    else
			    return selectedColor * borderAlpha;
	    });

	auto actBorderColor = Math::interpolate(selectedColor,
	    borderColor,
	    (double)drawItem.border);

	const auto &enabled =
	    label ? drawItem.labelEnabled : drawItem.enabled;
	auto alpha = (double)enabled * factor;

	auto finalBorderColor = actBorderColor * alpha;
	auto itemColor = selectedColor * alpha * fillAlpha;

	double highlight = 0.0;
	double anyHighlight = 0.0;
	auto markerInfo = plot.getMarkersInfo();
	for (auto &info : markerInfo) {
		auto allHighlight = 0.0;
		info.second.visit(
		    [&](int, const auto &info)
		    {
			    highlight += info.value.markerId == this->marker.idx
			                   ? 1.0
			                   : 0.0;
			    if (info.value.markerId != -1u)
				    allHighlight += info.weight;
		    });
		anyHighlight = std::max(anyHighlight, allHighlight);
	}

	auto highlightAlpha = 1 - (0.65 * anyHighlight) * (1 - highlight);
	finalBorderColor = (finalBorderColor * highlightAlpha);
	itemColor = (itemColor * highlightAlpha);

	return std::make_pair(finalBorderColor, itemColor);
}

Gfx::Color drawItem::getSelectedColor()
{
	auto orig = marker.color;

	auto gray = orig.desaturate().lightnessScaled(0.75);
	auto interpolated =
	    Math::interpolate(gray, orig, (double)marker.selected);

	return Math::interpolate(marker.color,
	    interpolated,
	    (double)plot.anySelected);
}
