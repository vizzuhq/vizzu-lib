#include "drawmarkerinfo.h"

#include "base/gfx/draw/infobubble.h"
#include "chart/main/style.h"
#include "chart/rendering/markers/abstractmarker.h"

namespace Vizzu::Draw
{

DrawMarkerInfo::MarkerDC::MarkerDC(const DrawMarkerInfo &parent,
    Gfx::ICanvas &canvas,
    const Geom::Rect &boundary,
    Content &content) :
    parent(parent),
    canvas(canvas)
{
	loadMarker(content);
	fillTextBox(content);
	calculateLayout();
	if (bubble.pos.x + bubble.size.x > boundary.size.x)
		calculateLayout(Geom::Point{-1, 0});
	if (bubble.pos.x <= boundary.pos.x)
		calculateLayout(Geom::Point{1, 0});
	if (bubble.pos.y + bubble.size.y > boundary.size.y)
		calculateLayout(Geom::Point{0, -1});
	if (bubble.pos.y <= boundary.pos.y)
		calculateLayout(Geom::Point{0, 1});
}

void DrawMarkerInfo::MarkerDC::draw(double weight)
{
	const Gfx::Color color1(1, 1, 1, weight);
	Gfx::Color color2(*parent.style.borderColor);
	Gfx::Color color3(*parent.style.shadowColor);
	color2.alpha = weight;
	color3.alpha *= weight;
	auto offset = *parent.style.dropShadow;
	canvas.setLineWidth(*parent.style.borderWidth);
	canvas.setLineColor(color2);
	canvas.setBrushColor(color1);
	canvas.beginDropShadow();
	canvas.setDropShadowBlur(2.0 * offset);
	canvas.setDropShadowColor(color3);
	canvas.setDropShadowOffset(Geom::Point{0, offset});
	Gfx::Draw::InfoBubble{canvas,
	    bubble,
	    *parent.style.borderRadius,
	    *parent.style.arrowSize,
	    arrow};
	canvas.endDropShadow();
	Gfx::Draw::InfoBubble{canvas,
	    bubble,
	    *parent.style.borderRadius,
	    *parent.style.arrowSize,
	    arrow};
	canvas.save();
	canvas.setClipRect(bubble);
	text << bubble.pos;
	text.draw(canvas, weight);
	canvas.restore();
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
	const auto &marker =
	    parent.plot->getMarkers()[cnt.markerId.value()];

	auto blendedMarker =
	    Draw::AbstractMarker::createInterpolated(parent.ctx(),
	        marker,
	        false);

	auto line =
	    blendedMarker.getLabelPos(Styles::MarkerLabel::Position::top,
	        parent.coordSys);
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
	bool was_first{};
	for (const auto &[cid, val] : *cnt.info)
		if (cid == parent.style.seriesName) {
			text << TextBox::Bkgnd(0) << TextBox::Fgnd(1);
			text << static_cast<Gfx::Font>(parent.style)
			     << static_cast<TextBox::Font>(
			            parent.style.fontSize->get() * 1.3)
			     << TextBox::bold << val;
			if (parent.style.layout
			    == Styles::Tooltip::Layout::multiLine)
				text << TextBox::NewLine();
			if (parent.style.layout
			    == Styles::Tooltip::Layout::singleLine)
				text << " / ";
			was_first = true;
			break;
		}

	for (const auto &[cid, val] : *cnt.info) {
		if (cid == parent.style.seriesName) continue;

		text << TextBox::Bkgnd(0) << TextBox::Fgnd(1);
		text << static_cast<Gfx::Font>(parent.style);
		if (parent.style.layout == Styles::Tooltip::Layout::singleLine
		    && std::exchange(was_first, true))
			text << ", ";
		text << cid << ": ";

		if (parent.style.layout == Styles::Tooltip::Layout::multiLine)
			text << TextBox::Tab();

		text << TextBox::bold << val;

		if (parent.style.layout == Styles::Tooltip::Layout::multiLine)
			text << TextBox::NewLine();
	}
}

void DrawMarkerInfo::MarkerDC::calculateLayout(Geom::Point hint)
{
	bubble.size = text.measure(canvas);
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

void DrawMarkerInfo::draw(Gfx::ICanvas &canvas,
    const Geom::Rect &boundary) const
{
	for (const auto &info : plot->getMarkersInfo()) {
		auto &&[cnt1, weight1] = info.second.get(false);
		if (!info.second.interpolates() && cnt1) {
			MarkerDC dc(*this, canvas, boundary, cnt1);
			dc.draw(weight1);
		}
		else if (info.second.interpolates()) {
			auto &&[cnt2, weight2] = info.second.get(true);
			if (!cnt1 && cnt2)
				fadeInMarkerInfo(canvas, boundary, cnt2, weight2);
			else if (cnt1 && !cnt2)
				fadeOutMarkerInfo(canvas, boundary, cnt1, weight1);
			else if (cnt1 && cnt2)
				moveMarkerInfo(canvas,
				    boundary,
				    cnt1,
				    weight1,
				    cnt2,
				    weight2);
		}
	}
}

void DrawMarkerInfo::fadeInMarkerInfo(Gfx::ICanvas &canvas,
    const Geom::Rect &boundary,
    Content &cnt,
    double weight) const
{
	MarkerDC dc(*this, canvas, boundary, cnt);
	dc.draw(weight);
}

void DrawMarkerInfo::fadeOutMarkerInfo(Gfx::ICanvas &canvas,
    const Geom::Rect &boundary,
    Content &cnt,
    double weight) const
{
	MarkerDC dc(*this, canvas, boundary, cnt);
	dc.draw(weight);
}

void DrawMarkerInfo::moveMarkerInfo(Gfx::ICanvas &canvas,
    const Geom::Rect &boundary,
    Content &cnt1,
    double weight1,
    Content &cnt2,
    double weight2) const
{
	MarkerDC dc1(*this, canvas, boundary, cnt1);
	MarkerDC dc2(*this, canvas, boundary, cnt2);
	dc1.interpolate(weight1, dc2, weight2);
	dc1.draw(weight1);
	dc2.draw(weight2);
}

}