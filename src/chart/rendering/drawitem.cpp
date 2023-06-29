#include "drawitem.h"

#include "base/geom/angle.h"
#include "base/text/smartstring.h"
#include "chart/rendering/drawlabel.h"
#include "chart/rendering/draworientedlabel.h"
#include "chart/rendering/items/circleitem.h"
#include "chart/rendering/items/drawitem.h"
#include "chart/rendering/items/connectingitem.h"
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
	if (static_cast<double>(marker.enabled) == 0) return;

	auto blended = DrawItem::createInterpolated(marker,
	    options,
	    plot.getStyle(),
	    coordSys,
	    plot.getMarkers(),
	    0);

	auto baseColor = *style.color * static_cast<double>(plot.anyAxisSet);

	if (static_cast<double>(blended.enabled) > 0) {
		if (static_cast<double>(plot.guides.x.guidelines) > 0) {
			auto lineColor =
			    baseColor * static_cast<double>(plot.guides.x.guidelines);
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
		if (static_cast<double>(plot.guides.y.guidelines) > 0) {
			blended.center.x = Math::interpolate(blended.center.x,
			    1.0,
			    static_cast<double>(options.polar));
			auto lineColor =
			    baseColor * static_cast<double>(plot.guides.y.guidelines);
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
	if (!shouldDrawMarkerBody()) return;

	if (options.shapeType.contains(Gen::ShapeType::line) 
		&& options.shapeType.contains(Gen::ShapeType::circle))
	{
		CircleItem circle(marker,
		    coordSys,
		    options, 
		    plot.getStyle());

		draw(circle, 1, false);

		marker.prevMainMarkerIdx.visit([&, this](int index, auto value){
			ConnectingItem line(marker,
					coordSys,
					options,
					plot.getStyle(),
					plot.getMarkers(),
					index,
					Gen::ShapeType::line);

			draw(line, value.weight, true);
		});
	}
	else 
	{
		auto drawMarker = [&, this](int index, ::Anim::Weighted<uint64_t> value) {
			auto blended0 = DrawItem::createInterpolated(marker,
				options,
				plot.getStyle(),
				coordSys,
				plot.getMarkers(),
				index);
	
			auto lineFactor = 
				options.shapeType.factor<double>(Gen::ShapeType::line);

			draw(blended0, value.weight * (1 - lineFactor) * (1 - lineFactor), false);
			draw(blended0, value.weight * sqrt(lineFactor), true);
		};

		auto containsConnected = 
			options.shapeType.contains(Gen::ShapeType::line) 
			|| options.shapeType.contains(Gen::ShapeType::area);

		auto containsSingle = 
			options.shapeType.contains(Gen::ShapeType::rectangle) 
			|| options.shapeType.contains(Gen::ShapeType::circle);

		if (containsConnected)
		{
			if (containsSingle) 
			{
				auto lineIndex = 
					Gen::isConnecting(options.shapeType.get(0).value)
					? 0 : 1;

				drawMarker(lineIndex, ::Anim::Weighted<uint64_t>(0));
			}
			else marker.prevMainMarkerIdx.visit(drawMarker);
		}
		else drawMarker(0, ::Anim::Weighted<uint64_t>(0));
	}
}

void drawItem::drawLabel()
{
	if (static_cast<double>(marker.enabled) == 0) return;

	auto blended = DrawItem::createInterpolated(marker,
	    options,
	    plot.getStyle(),
	    coordSys,
	    plot.getMarkers(),
	    0);

	drawLabel(blended, 0);
	drawLabel(blended, 1);
}

bool drawItem::shouldDrawMarkerBody()
{
	bool enabled = static_cast<double>(marker.enabled) > 0;
	if (options.shapeType.factor<Math::FuzzyBool>(
	        Gen::ShapeType::area) != false) {
		const auto *prev0 = ConnectingItem::getPrev(marker, plot.getMarkers(),
		    0);

		const auto *prev1 = ConnectingItem::getPrev(marker, plot.getMarkers(),
		    1);

		if (prev0) enabled |= static_cast<double>(prev0->enabled) > 0;
		if (prev1) enabled |= static_cast<double>(prev1->enabled) > 0;
	}
	return enabled;
}

void drawItem::draw(const DrawItem &drawItem,
    double factor,
    bool line)
{
	if (static_cast<double>(drawItem.enabled) == 0 || factor == 0) return;

	painter.setPolygonToCircleFactor(
	    line ? 0.0 : static_cast<double>(drawItem.morphToCircle));
	painter.setPolygonStraightFactor(static_cast<double>(drawItem.linear));
	painter.setResMode(ResolutionMode::High);

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
			    static_cast<double>(drawItem.linear),
			    colors.second,
			    colors.second * static_cast<double>(drawItem.connected));
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
	if (static_cast<double>(drawItem.labelEnabled) == 0) return;

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

	auto centered = labelStyle.position->factor<double>(
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
	    static_cast<double>(drawItem.border));

	const auto &enabled =
	    label ? drawItem.labelEnabled : drawItem.enabled;
	auto alpha = static_cast<double>(enabled) * factor;

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
	    Math::interpolate(gray, orig, static_cast<double>(marker.selected));

	return Math::interpolate(marker.color,
	    interpolated,
	    static_cast<double>(plot.anySelected));
}
