#include "drawmarkerinfo.h"

#include "base/io/log.h"
#include "base/gfx/draw/roundedrect.h"
#include "chart/rendering/items/blendeditem.h"

using namespace Vizzu;
using namespace Vizzu::Draw;

drawMarkerInfo::drawMarkerInfo(Geom::Rect plot, Gfx::ICanvas & canvas, const Diag::Diagram &diagram)
	: diagram(diagram), coordSystem(nullptr), canvas(canvas), lineHeight(0)
{
	auto coordSys = Draw::CoordinateSystem(
		plot, diagram.getOptions()->angle.get(),
		diagram.getOptions()->polar.get(), diagram.keepAspectRatio);
	coordSystem = &coordSys;
	for(auto& info : diagram.getMarkersInfo()) {
		if (info.second.count == 0)
			continue;
		auto weight1 = info.second.values[0].weight;
		auto& cnt1 = info.second.values[0].value;
		if (info.second.count == 1 && cnt1) {
			auto control = getDataPoint(cnt1);
			auto position = calculatePosition(control);
			draw(position, cnt1, weight1);
		}
		else if (info.second.count == 2) {
			auto weight2 = info.second.values[1].weight;
			auto& cnt2 = info.second.values[1].value;
			if (!cnt1 && cnt2)
				fadeInMarkerInfo(cnt2, weight2);
			else if (cnt1 && !cnt2)
				fadeOutMarkerInfo(cnt1, weight1);
			else if (cnt1 && cnt2)
				moveMarkerInfo(cnt1, weight1, cnt2, weight2);
			else
				throw std::logic_error("invalid marker info combination");
		}
	}
}

void drawMarkerInfo::fadeInMarkerInfo(Content& cnt, double weight) {
	auto control = getDataPoint(cnt);
	auto position = calculatePosition(control);
	draw(position, cnt, weight);
}

void drawMarkerInfo::fadeOutMarkerInfo(Content& cnt, double weight) {
	auto control = getDataPoint(cnt);
	auto position = calculatePosition(control);
	draw(position, cnt, weight);
}

void drawMarkerInfo::moveMarkerInfo(Content& cnt1, double weight1, Content& cnt2, double weight2) {
	auto control = getDataPoint(cnt1);
	auto position1 = calculatePosition(control);
	control = getDataPoint(cnt2);
	auto position2 = calculatePosition(control);
	auto interpolated = position1 * weight1 + position2 * weight2;
	draw(interpolated, cnt1, weight1);
	draw(interpolated, cnt2, weight2);
}

Geom::Line drawMarkerInfo::getDataPoint(Content& cnt) {
	auto& marker = diagram.getMarkers()[cnt.markerId];
	Draw::BlendedDrawItem blendedMarker(
		marker, *diagram.getOptions(), diagram.getStyle(),
		*coordSystem, diagram.getMarkers(), 0);
	auto line = blendedMarker.getLabelPos(
		Styles::MarkerLabel::Position::top, *coordSystem);
	return line;
}

Geom::Point drawMarkerInfo::calculatePosition(Geom::Line& control) {
	return control.begin;
}

Geom::Size drawMarkerInfo::calculateTextSize(Content& cnt) {
	Geom::Size result;
	for(auto& info : cnt.content) {
		canvas.setFont(Gfx::Font(14));
		auto tmp = canvas.textBoundary(info.first + ": " + info.second);
		if (tmp.x > result.x)
			result.x = tmp.x;
		result.y += tmp.y + tmp.y / 4;
		if (tmp.y + tmp.y / 4 > lineHeight)
			lineHeight = tmp.y + tmp.y / 4;
	}
	return result;
}

void drawMarkerInfo::draw(Geom::Point& position, Content& cnt, double weight) {
	Gfx::Color color(0.80, 0.80, 0.80, weight);
	canvas.setLineWidth(3);
	canvas.setBrushColor(Gfx::Color(0, 0, 0, 0));
	canvas.setLineColor(color);
	canvas.circle(Geom::Circle(position, 7));
	auto size = calculateTextSize(cnt);
	size.x += 20, size.y += 10;
	Geom::Rect background(position, size);
	drawBackground(background, 4, weight);
	Geom::Point textPos(position.x + 20 / 2, position.y + 10 / 2);
	drawText(textPos, cnt, weight);
}

void drawMarkerInfo::drawBackground(Geom::Rect& rect, double radius, double weight) {
	Gfx::Color color1(1, 1, 1, weight);
	Gfx::Color color2(0.85, 0.85, 0.85, weight);
	canvas.setLineWidth(1);
	canvas.setLineColor(color2);
	canvas.setBrushColor(color1);
	Gfx::Draw::RoundedRect {canvas, rect, radius};
}

void drawMarkerInfo::drawText(Geom::Point& position, Content& cnt, double weight) {
	canvas.setFont(Gfx::Font(14));
	Gfx::Color color(0.2, 0.2, 0.2, weight);
	canvas.setTextColor(color);
	Geom::Rect rect(position, Geom::Point(1000, 1000));
	for(auto& info : cnt.content) {
		canvas.text(rect, info.first + ": " + info.second);
		rect.pos.y += lineHeight;
	}
}