#include "drawlabel.h"

namespace Vizzu::Draw
{

DrawLabel::DrawLabel(const DrawingContext &context,
    const Geom::TransformedRect &rect,
    const std::string &text,
    const Styles::Label &style,
    const Util::EventDispatcher::event_ptr &onDraw,
    std::unique_ptr<Util::EventTarget> eventTarget,
    Options options) :
    DrawingContext(context),
    style(style),
    onDraw(onDraw)
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

	contentRect = style.contentRect(relRect, style.calculatedSize());

	canvas.setFont(Gfx::Font{style});
	if (options.setColor)
		canvas.setTextColor(*style.color * options.alpha);

	auto textSize = canvas.textBoundary(text);
	auto alignSize = textSize;
	alignSize.x = std::min(alignSize.x, contentRect.size.x);
	auto textRect = alignText(alignSize);
	textRect.size = textSize;

	auto transform =
	    rect.transform * Geom::AffineTransform(textRect.bottomLeft());

	if (options.flip)
		transform = transform
		          * Geom::AffineTransform(textRect.size, 1.0, -M_PI);

	Geom::TransformedRect trRect;
	trRect.transform = transform;
	trRect.size = textRect.size;

	if (this->onDraw->invoke(
	        Events::OnTextDrawEvent(*eventTarget, trRect, text))) {
		canvas.transform(transform);

		canvas.text(Geom::Rect(Geom::Point(), textRect.size), text);

		renderedChart.emplace(trRect, std::move(eventTarget));
	}

	canvas.restore();
}

double DrawLabel::getHeight(const Styles::Label &style,
    Gfx::ICanvas &canvas)
{
	const Gfx::Font font(style);
	canvas.setFont(font);
	auto textHeight = canvas.textBoundary("").y;
	return style.paddingTop->get(textHeight, font.size)
	     + style.paddingBottom->get(textHeight, font.size)
	     + textHeight;
}

Geom::Rect DrawLabel::alignText(const Geom::Size &textSize)
{
	Geom::Rect res;
	res.size = textSize;
	res.pos = contentRect.pos;

	res.pos.x = style.textAlign->combine<double>(
	    [this, &textSize](int,
	        const Styles::Text::TextAlign &align) -> double
	    {
		    switch (align) {
		    case Styles::Text::TextAlign::left:
			    return contentRect.left();

		    case Styles::Text::TextAlign::right:
			    return contentRect.right() - textSize.x;

		    default:
		    case Styles::Text::TextAlign::center:
			    return contentRect.center().x - textSize.x / 2.0;
		    }
	    });

	return res;
}

}