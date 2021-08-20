#include "drawmarkerinfo.h"

#include "base/io/log.h"
#include "base/gfx/draw/infobubble.h"
#include "chart/rendering/items/blendeditem.h"
#include "chart/main/style.h"

using namespace Vizzu;
using namespace Vizzu::Draw;

drawMarkerInfo::drawMarkerInfo(
	Geom::Rect plot, Gfx::ICanvas & canvas,
	const Diag::Diagram &diagram,
	const Styles::MarkerInfo &style)
	: diagram(diagram), coordSystem(nullptr), canvas(canvas),
	style(style), plot(plot)
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
			TextBox text(canvas, (Gfx::Font)Styles::Chart::defaultFont);
			fillTextBox(cnt1, text);
			auto position = calculatePosition(control, text.size());
			highlightDataPoint(control.begin, weight1);
			draw(position, text, weight1);
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
	TextBox text(canvas, (Gfx::Font)Styles::Chart::defaultFont);
	fillTextBox(cnt, text);
	auto position = calculatePosition(control, text.size());
	highlightDataPoint(control.begin, weight);
	draw(position, text, weight);
}

void drawMarkerInfo::fadeOutMarkerInfo(Content& cnt, double weight) {
	auto control = getDataPoint(cnt);
	TextBox text(canvas, (Gfx::Font)Styles::Chart::defaultFont);
	fillTextBox(cnt, text);
	auto position = calculatePosition(control, text.size());
	highlightDataPoint(control.begin, weight);
	draw(position, text, weight);
}

void drawMarkerInfo::moveMarkerInfo(Content& cnt1, double weight1, Content& cnt2, double weight2) {
	auto control1 = getDataPoint(cnt1);
	TextBox text1(canvas, (Gfx::Font)Styles::Chart::defaultFont);
	fillTextBox(cnt1, text1);
	auto position1 = calculatePosition(control1, text1.size());
	auto control2 = getDataPoint(cnt2);
	TextBox text2(canvas, (Gfx::Font)Styles::Chart::defaultFont);
	fillTextBox(cnt2, text2);
	auto position2 = calculatePosition(control2, text2.size());
	auto interpolated = position1 * weight1 + position2 * weight2;
	highlightDataPoint(control1.begin, weight1);
	draw(interpolated, text1, weight1);
	highlightDataPoint(control2.begin, weight2);
	draw(interpolated, text2, weight2);
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

Geom::Point drawMarkerInfo::calculatePosition(const Geom::Line& control, const Geom::Size&) {
	return control.extend(*style.distance).end;
}

void drawMarkerInfo::draw(const Geom::Point& position, TextBox& text, double weight) {
	Gfx::Color color1(1, 1, 1, weight);
	Gfx::Color color2(*style.borderColor);
	Gfx::Color color3(0, 0, 0, weight);
	color2.alpha = weight;
	double offset = *style.dropshadow;
	canvas.setLineWidth(*style.borderWidth);
	canvas.setLineColor(color2);
	canvas.setBrushColor(color1);
	canvas.beginDropShadow();
	canvas.setDropShadowBlur(5);
	canvas.setDropShadowColor(color3);
	canvas.setDropShadowOffset(Geom::Point(0, offset * 4 - weight * offset * 3));
	Geom::Rect rect;
	rect.size = text.size();
	rect.pos.x = position.x - text.size().x / 2;
	rect.pos.y = position.y - text.size().y - *style.pointerSize;
	Gfx::Draw::InfoBubble {canvas, rect, *style.rounding, *style.pointerSize, position};
	canvas.endDropShadow();
	Gfx::Draw::InfoBubble {canvas, rect, *style.rounding, *style.pointerSize, position};
	text.draw(rect.pos, weight);
}

void drawMarkerInfo::fillTextBox(Content& cnt, TextBox& text) {
	double r = *style.rounding * 2;
	text.padding(r, r, r, r);
	text.addColor(*style.backgroundColor);
	text.addColor(*style.textColor);
	if (style.style == Styles::MarkerInfo::Style::multiLine)
		text.addTabulator(TextBox::Tab::Auto);
	bool first = true;
	for(auto& info : cnt.content) {
		if (style.style == Styles::MarkerInfo::Style::multiLine) {
			text << TextBox::BgColor(0) << TextBox::FgColor(1);
			text << TextBox::Font(style.fontSize->get(), false, false);
			text << info.first << ": " << TextBox::Tab();
			text << TextBox::BgColor(0) << TextBox::FgColor(1);
			text << TextBox::Font(14, true, false);
			text << info.second << TextBox::NewLine();
		}
		if (style.style == Styles::MarkerInfo::Style::singleLine) {
			text << TextBox::BgColor(0) << TextBox::FgColor(1);
			text << TextBox::Font(style.fontSize->get(), false, false);
			if (!first)
				text << " / ";
			text << info.first << ": ";
			text << TextBox::BgColor(0) << TextBox::FgColor(1);
			text << TextBox::Font(style.fontSize->get(), true, false);
			text << info.second;
			first = false;
		}
	}
}

void drawMarkerInfo::highlightDataPoint(const Geom::Point& position, double weight) {
	canvas.setLineWidth(3);
	canvas.setBrushColor(Gfx::Color(0, 0, 0, 0));
	Gfx::Color color(*style.borderColor);
	color.alpha = weight;
	canvas.setLineColor(color);
	canvas.circle(Geom::Circle(position, *style.markerSize));
}
