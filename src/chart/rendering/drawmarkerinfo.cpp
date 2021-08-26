#include "drawmarkerinfo.h"

#include "base/io/log.h"
#include "base/gfx/draw/infobubble.h"
#include "chart/rendering/items/blendeditem.h"
#include "chart/main/style.h"

using namespace Vizzu;
using namespace Vizzu::Draw;

drawMarkerInfo::MarkerDC::MarkerDC(drawMarkerInfo& parent, Content& content)
	: parent(parent)
{
	loadMarker(content);
	fillTextBox(content);
	calculateLayout();
	if (bubble.pos.x + bubble.size.x > parent.layout.boundary.size.x)
		calculateLayout(Geom::Point(-1, 0));
	if (bubble.pos.x <= parent.layout.boundary.pos.x)
		calculateLayout(Geom::Point(1, 0));
	if (bubble.pos.y + bubble.size.y > parent.layout.boundary.size.y)
		calculateLayout(Geom::Point(0, -1));
	if (bubble.pos.y <= parent.layout.boundary.pos.y)
		calculateLayout(Geom::Point(0, 1));
}

void drawMarkerInfo::MarkerDC::draw(double weight) {
	Gfx::Color color1(1, 1, 1, weight);
	Gfx::Color color2(*parent.style.borderColor);
	Gfx::Color color3(0, 0, 0, weight);
	color2.alpha = weight;
	double offset = *parent.style.dropshadow;
	parent.canvas.setLineWidth(*parent.style.borderWidth);
	parent.canvas.setLineColor(color2);
	parent.canvas.setBrushColor(color1);
	parent.canvas.beginDropShadow();
	parent.canvas.setDropShadowBlur(5);
	parent.canvas.setDropShadowColor(color3);
	parent.canvas.setDropShadowOffset(Geom::Point(0, offset * 4 - weight * offset * 3));
	Gfx::Draw::InfoBubble {parent.canvas, bubble, *parent.style.rounding, *parent.style.pointerSize, arrow};
	parent.canvas.endDropShadow();
	Gfx::Draw::InfoBubble {parent.canvas, bubble, *parent.style.rounding, *parent.style.pointerSize, arrow};
	text << bubble.pos;
	text.draw(parent.canvas, weight);
}

void drawMarkerInfo::MarkerDC::highlight(double weight) {
	parent.canvas.setLineWidth(3);
	parent.canvas.setBrushColor(Gfx::Color(0, 0, 0, 0));
	Gfx::Color color(*parent.style.borderColor);
	color.alpha = weight;
	parent.canvas.setLineColor(color);
	parent.canvas.circle(Geom::Circle(dataPoint, *parent.style.markerSize));
}

void drawMarkerInfo::MarkerDC::interpolate(double weight1, MarkerDC& other, double weight2) {
	arrow = arrow * weight1 + other.arrow * weight2;
	bubble = bubble * weight1 + other.bubble * weight2;
	other.arrow = arrow;
	other.bubble = bubble;
}

void drawMarkerInfo::MarkerDC::loadMarker(Content& cnt) {
	auto& marker = parent.diagram.getMarkers()[cnt.markerId];
	Draw::BlendedDrawItem blendedMarker(
		marker, *parent.diagram.getOptions(), parent.diagram.getStyle(),
		*parent.coordSystem, parent.diagram.getMarkers(), 0);
	auto line = blendedMarker.getLabelPos(
		Styles::MarkerLabel::Position::top, *parent.coordSystem);
	dataPoint = line.begin;
	labelDir = line.end - line.begin;
}

void drawMarkerInfo::MarkerDC::fillTextBox(Content& cnt) {
	double r = *parent.style.rounding * 2;
	text << TextBox::Padding(r, r, r, r);
	text << *parent.style.backgroundColor;
	text << *parent.style.textColor;
	if (parent.style.style == Styles::MarkerInfo::Style::multiLine)
		text << TextBox::TabPos(0);
	bool first = true;
	for(auto& info : cnt.content) {
		if (parent.style.style == Styles::MarkerInfo::Style::multiLine) {
			text << TextBox::Bkgnd(0) << TextBox::Fgnd(1);
			text << (Gfx::Font)parent.style;
			text << info.first << ": " << TextBox::Tab();
			text << TextBox::bold;
			text << info.second << TextBox::NewLine();
		}
		if (parent.style.style == Styles::MarkerInfo::Style::singleLine) {
			text << TextBox::Bkgnd(0) << TextBox::Fgnd(1);
			text << (Gfx::Font)parent.style;
			if (!first)
				text << " / ";
			text << info.first << ": ";
			text << TextBox::bold;
			text << info.second;
			first = false;
		}
	}
}

void drawMarkerInfo::MarkerDC::calculateLayout(Geom::Point hint) {
	bubble.size = text.measure(parent.canvas);
	if (hint.isNull()) {
		// orientation: horizontal, right
		if ((labelDir.x > 0 && labelDir.y > 0 && labelDir.x > labelDir.y) ||
			(labelDir.x > 0 && labelDir.y <= 0 && labelDir.x > -labelDir.y))
		{
			hint.x = 1; hint.y = 0;
		}
		// orientation: vertiacal, bottom
		if ((labelDir.x > 0 && labelDir.y > 0 && labelDir.x <= labelDir.y) ||
			(labelDir.x <= 0 && labelDir.y > 0 && -labelDir.x <= labelDir.y))
		{
			hint.x = 0; hint.y = 1;
		}
		// orientation: horizontal, left
		if ((labelDir.x <= 0 && labelDir.y > 0 && -labelDir.x > labelDir.y) ||
			(labelDir.x <= 0 && labelDir.y <= 0 && -labelDir.x > -labelDir.y))
		{
			hint.x = -1; hint.y = 0;
		}
		// orientation: vertical, top
		if ((labelDir.x <= 0 && labelDir.y <= 0 && -labelDir.x <= -labelDir.y) ||
			(labelDir.x > 0 && labelDir.y <= 0 && labelDir.x <= -labelDir.y))
		{
			hint.x = 0; hint.y = -1;
		}
	}
	Geom::Line control(dataPoint, dataPoint + hint);
	arrow = control.extend(*parent.style.distance).end;
	bubble.pos.x = arrow.x - bubble.size.x / 2;
	bubble.pos.y = arrow.y - bubble.size.y / 2;
	if (hint.x == 1)
		bubble.pos.x = arrow.x + *parent.style.pointerSize;
	if (hint.x == -1)
		bubble.pos.x = arrow.x - bubble.size.x - *parent.style.pointerSize;
	if (hint.y == 1)
		bubble.pos.y = arrow.y + *parent.style.pointerSize;
	if (hint.y == -1)
		bubble.pos.y = arrow.y - bubble.size.y - *parent.style.pointerSize;
}

drawMarkerInfo::drawMarkerInfo(
	const Layout& layout, Gfx::ICanvas & canvas, const Diag::Diagram &diagram)
	: layout(layout), canvas(canvas), diagram(diagram),
	  coordSystem(nullptr), style(diagram.getStyle().info)
{
	auto coordSys = Draw::CoordinateSystem(
		layout.plotArea, diagram.getOptions()->angle.get(),
		diagram.getOptions()->polar.get(), diagram.keepAspectRatio);
	coordSystem = &coordSys;
	for(auto& info : diagram.getMarkersInfo()) {
		if (info.second.count == 0)
			continue;
		auto weight1 = info.second.values[0].weight;
		auto& cnt1 = info.second.values[0].value;
		if (info.second.count == 1 && cnt1) {
			MarkerDC dc(*this, cnt1);
			dc.highlight(weight1);
			dc.draw(weight1);
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
	MarkerDC dc(*this, cnt);
	dc.highlight(weight);
	dc.draw(weight);
}

void drawMarkerInfo::fadeOutMarkerInfo(Content& cnt, double weight) {
	MarkerDC dc(*this, cnt);
	dc.highlight(weight);
	dc.draw(weight);
}

void drawMarkerInfo::moveMarkerInfo(Content& cnt1, double weight1, Content& cnt2, double weight2) {
	MarkerDC dc1(*this, cnt1);
	MarkerDC dc2(*this, cnt2);
	dc1.interpolate(weight1, dc2, weight2);
	dc1.highlight(weight1);
	dc1.draw(weight1);
	dc2.highlight(weight2);
	dc2.draw(weight2);
}
