#include "drawmarkerinfo.h"

#include "base/gfx/draw/infobubble.h"
#include "chart/main/style.h"
#include "chart/rendering/markers/abstractmarker.h"

using namespace Vizzu;
using namespace Vizzu::Draw;

DrawMarkerInfo::MarkerDC::MarkerDC(DrawMarkerInfo &parent,
    Content &content) :
    parent(parent)
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

void DrawMarkerInfo::MarkerDC::draw(double weight)
{
	const Gfx::Color color1(1, 1, 1, weight);
	Gfx::Color color2(*parent.style.borderColor);
	Gfx::Color color3(*parent.style.shadowColor);
	color2.alpha = weight;
	color3.alpha *= weight;
	auto offset = *parent.style.dropShadow;
	parent.canvas.setLineWidth(*parent.style.borderWidth);
	parent.canvas.setLineColor(color2);
	parent.canvas.setBrushColor(color1);
	parent.canvas.beginDropShadow();
	parent.canvas.setDropShadowBlur(2 * offset);
	parent.canvas.setDropShadowColor(color3);
	parent.canvas.setDropShadowOffset(Geom::Point(0, offset));
	Gfx::Draw::InfoBubble{parent.canvas,
	    bubble,
	    *parent.style.borderRadius,
	    *parent.style.arrowSize,
	    arrow};
	parent.canvas.endDropShadow();
	Gfx::Draw::InfoBubble{parent.canvas,
	    bubble,
	    *parent.style.borderRadius,
	    *parent.style.arrowSize,
	    arrow};
	parent.canvas.save();
	parent.canvas.setClipRect(bubble);
	text << bubble.pos;
	text.draw(parent.canvas, weight);
	parent.canvas.restore();
}

void DrawMarkerInfo::MarkerDC::interpolate(double weight1,
    MarkerDC &other,
    double weight2)
{
	arrow = arrow * weight1 + other.arrow * weight2;
	bubble = bubble * weight1 + other.bubble * weight2;
	other.arrow = arrow;
	other.bubble = bubble;
}

void DrawMarkerInfo::MarkerDC::loadMarker(Content &cnt)
{
	const auto &marker = parent.plot.getMarkers()[cnt.markerId.value()];

	auto blendedMarker = Draw::AbstractMarker::createInterpolated(marker,
	    *parent.plot.getOptions(),
	    parent.plot.getStyle(),
	    *parent.coordSystem,
	    parent.plot.getMarkers(),
	    0);

	auto line =
	    blendedMarker.getLabelPos(Styles::MarkerLabel::Position::top,
	        *parent.coordSystem);
	dataPoint = line.begin;
	labelDir = line.end - line.begin;
}

void DrawMarkerInfo::MarkerDC::fillTextBox(Content &cnt)
{
	auto r = *parent.style.borderRadius * 2;
	text << TextBox::Padding(r, r, r, r) << TextBox::LineSpacing(1.5);
	text << Gfx::Color(1, 1, 1, 0);
	text << *parent.style.color;
	if (parent.style.layout == Styles::Tooltip::Layout::multiLine)
		text << TextBox::TabPos(0);
	int counter = 0;
	std::string firstContent;
	for (const auto &info : cnt.content) {
		if (info.first == parent.style.seriesName) {
			firstContent = info.second;
		}
	}
	for (const auto &info : cnt.content) {
		if (parent.style.layout
		    == Styles::Tooltip::Layout::multiLine) {
			if (counter == 0 && !firstContent.empty()) {
				text << TextBox::Bkgnd(0) << TextBox::Fgnd(1);
				text << static_cast<Gfx::Font>(parent.style)
				     << TextBox::Font(
				            parent.style.fontSize->get() * 1.3);
				text << TextBox::bold << firstContent
				     << TextBox::NewLine();
			}
			else {
				text << TextBox::Bkgnd(0) << TextBox::Fgnd(1);
				text << static_cast<Gfx::Font>(parent.style);
				text << info.first << ": " << TextBox::Tab();
				text << TextBox::bold;
				text << info.second << TextBox::NewLine();
			}
		}
		if (parent.style.layout
		    == Styles::Tooltip::Layout::singleLine) {
			if (counter == 0 && !firstContent.empty()) {
				text << TextBox::Bkgnd(0) << TextBox::Fgnd(1);
				text << static_cast<Gfx::Font>(parent.style)
				     << TextBox::Font(
				            parent.style.fontSize->get() * 1.3);
				text << TextBox::bold << firstContent << " / ";
			}
			else {
				text << TextBox::Bkgnd(0) << TextBox::Fgnd(1);
				text << static_cast<Gfx::Font>(parent.style);
				if ((firstContent.empty() && counter != 0)
				    || (!firstContent.empty() && counter != 1))
					text << ", ";
				text << info.first << ": ";
				text << TextBox::bold;
				text << info.second;
			}
		}
		counter++;
	}
}

void DrawMarkerInfo::MarkerDC::calculateLayout(Geom::Point hint)
{
	bubble.size = text.measure(parent.canvas);
	if (hint.isNull()) {
		// orientation: horizontal, right
		if ((labelDir.x > 0 && labelDir.y > 0
		        && labelDir.x > labelDir.y)
		    || (labelDir.x > 0 && labelDir.y <= 0
		        && labelDir.x > -labelDir.y)) {
			hint.x = 1;
			hint.y = 0;
		}
		// orientation: vertiacal, bottom
		if ((labelDir.x > 0 && labelDir.y > 0
		        && labelDir.x <= labelDir.y)
		    || (labelDir.x <= 0 && labelDir.y > 0
		        && -labelDir.x <= labelDir.y)) {
			hint.x = 0;
			hint.y = 1;
		}
		// orientation: horizontal, left
		if ((labelDir.x <= 0 && labelDir.y > 0
		        && -labelDir.x > labelDir.y)
		    || (labelDir.x <= 0 && labelDir.y <= 0
		        && -labelDir.x > -labelDir.y)) {
			hint.x = -1;
			hint.y = 0;
		}
		// orientation: vertical, top
		if ((labelDir.x <= 0 && labelDir.y <= 0
		        && -labelDir.x <= -labelDir.y)
		    || (labelDir.x > 0 && labelDir.y <= 0
		        && labelDir.x <= -labelDir.y)) {
			hint.x = 0;
			hint.y = -1;
		}
	}
	const Geom::Line control(dataPoint, dataPoint + hint);
	arrow = control.extend(*parent.style.distance).end;
	bubble.pos.x = arrow.x - bubble.size.x / 2;
	bubble.pos.y = arrow.y - bubble.size.y / 2;
	if (hint.x == 1) bubble.pos.x = arrow.x + *parent.style.arrowSize;
	if (hint.x == -1)
		bubble.pos.x =
		    arrow.x - bubble.size.x - *parent.style.arrowSize;
	if (hint.y == 1) bubble.pos.y = arrow.y + *parent.style.arrowSize;
	if (hint.y == -1)
		bubble.pos.y =
		    arrow.y - bubble.size.y - *parent.style.arrowSize;
}

DrawMarkerInfo::DrawMarkerInfo(const Layout &layout,
    Gfx::ICanvas &canvas,
    const Gen::Plot &plot) :
    layout(layout),
    canvas(canvas),
    plot(plot),
    style(plot.getStyle().tooltip)
{
	auto coordSys = Draw::CoordinateSystem(layout.plotArea,
	    plot.getOptions()->angle,
	    plot.getOptions()->coordSystem,
	    plot.keepAspectRatio);
	coordSystem = &coordSys;
	for (const auto &info : plot.getMarkersInfo()) {
		if (info.second.count == 0) continue;
		auto weight1 = info.second.values[0].weight;
		const auto &cnt1 = info.second.values[0].value;
		if (info.second.count == 1 && cnt1) {
			MarkerDC dc(*this, cnt1);
			dc.draw(weight1);
		}
		else if (info.second.count == 2) {
			auto weight2 = info.second.values[1].weight;
			const auto &cnt2 = info.second.values[1].value;
			if (!cnt1 && cnt2)
				fadeInMarkerInfo(cnt2, weight2);
			else if (cnt1 && !cnt2)
				fadeOutMarkerInfo(cnt1, weight1);
			else if (cnt1 && cnt2)
				moveMarkerInfo(cnt1, weight1, cnt2, weight2);
			else
				throw std::logic_error(
				    "invalid marker info combination");
		}
	}
}

void DrawMarkerInfo::fadeInMarkerInfo(Content &cnt, double weight)
{
	MarkerDC dc(*this, cnt);
	dc.draw(weight);
}

void DrawMarkerInfo::fadeOutMarkerInfo(Content &cnt, double weight)
{
	MarkerDC dc(*this, cnt);
	dc.draw(weight);
}

void DrawMarkerInfo::moveMarkerInfo(Content &cnt1,
    double weight1,
    Content &cnt2,
    double weight2)
{
	MarkerDC dc1(*this, cnt1);
	MarkerDC dc2(*this, cnt2);
	dc1.interpolate(weight1, dc2, weight2);
	dc1.draw(weight1);
	dc2.draw(weight2);
}
