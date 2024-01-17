#include "drawlabel.h"

namespace Vizzu::Draw
{

void DrawLabel::draw(Gfx::ICanvas &canvas,
    const Geom::TransformedRect &rect,
    const std::string &text,
    const Styles::Label &style,
    Util::EventDispatcher::Event &onDraw,
    std::unique_ptr<Util::EventTarget> eventTarget,
    Options options) const
{
	auto relRect = Geom::Rect{Geom::Point(), rect.size};

	if (!style.backgroundColor->isTransparent()) {
		canvas.save();
		canvas.setBrushColor(*style.backgroundColor);
		canvas.setLineColor(*style.backgroundColor);
		canvas.transform(rect.transform);
		canvas.rectangle(relRect);
		canvas.restore();
	}

	canvas.save();

	auto contentRect =
	    style.contentRect(relRect, style.calculatedSize());

	auto font = Gfx::Font{style};
	canvas.setFont(font);
	if (options.setColor)
		canvas.setTextColor(*style.color * options.alpha);

	auto textSize = Gfx::ICanvas::textBoundary(font, text);
	auto alignSize = textSize;
	alignSize.x = std::min(alignSize.x, contentRect.size.x);
	auto [textRect, d] = alignText(contentRect, style, alignSize);
	textRect.size = textSize;

	auto transform =
	    rect.transform * Geom::AffineTransform(textRect.bottomLeft());

	if (options.flip)
		transform = transform
		          * Geom::AffineTransform(textRect.size, 1.0, -M_PI);

	Geom::TransformedRect trRect;
	trRect.transform = transform;
	trRect.size = textRect.size;

	if (onDraw.invoke(
	        Events::OnTextDrawEvent{*eventTarget, trRect, text})) {
		canvas.transform(transform);

		canvas.text(Geom::Rect(Geom::Point(), textRect.size), text);

		renderedChart.emplace(trRect, std::move(eventTarget));
	}

	canvas.restore();
}

double DrawLabel::getHeight(const Styles::Label &style)
{
	const Gfx::Font font(style);
	auto textHeight = Gfx::ICanvas::textBoundary(font, "").y;
	return style.paddingTop->get(textHeight, font.size)
	     + style.paddingBottom->get(textHeight, font.size)
	     + textHeight;
}

std::pair<Geom::Rect, double> DrawLabel::alignText(
    const Geom::Rect &contentRect,
    const Styles::Label &style,
    const Geom::Size &textSize)
{
	Geom::Rect res{contentRect.pos, textSize};

	auto align = style.textAlign->calculate<double>();
	res.pos.x = contentRect.center().x - textSize.x / 2.0
	          + (contentRect.size.x - textSize.x) * align;

	return {res, align};
}

}