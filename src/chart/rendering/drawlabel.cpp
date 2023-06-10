#include "drawlabel.h"

#include "base/io/log.h"

using namespace Vizzu;
using namespace Vizzu::Draw;

drawLabel::drawLabel(const Geom::Rect &rect,
    const std::string &text,
    const Styles::Label &style,
    const Util::EventDispatcher::event_ptr &onDraw,
    Events::Events::OnTextDrawParam &&eventObj,
    Gfx::ICanvas &canvas,
    Options options) :
    text(text),
    style(style),
    onDraw(onDraw),
    canvas(canvas)
{
	canvas.save();
	
	if (!style.backgroundColor->isTransparent())
	{
		canvas.setBrushColor(*style.backgroundColor);
		canvas.setLineColor(*style.backgroundColor);
		canvas.rectangle(rect);
	}

	contentRect = style.contentRect(rect, style.calculatedSize());

	canvas.setFont(Gfx::Font(style));
	if (options.setColor) canvas.setTextColor(*style.color * options.alpha);

	auto textSize = getTextSize();
	auto textRect = alignText(textSize);

	eventObj.text = text;
	eventObj.rect = textRect;

	auto transform = Geom::AffineTransform(textRect.bottomLeft());
	if (options.flip)
		transform = transform * Geom::AffineTransform(textRect.size, 1.0, -M_PI);

	canvas.transform(transform);

	if (this->onDraw->invoke(std::move(eventObj))) 
		canvas.text(Geom::Rect(Geom::Point(), textRect.size), text);

	canvas.restore();
}

double drawLabel::getHeight(const Styles::Label &style,
    Gfx::ICanvas &canvas)
{
	Gfx::Font font(style);
	canvas.setFont(font);
	auto textHeight = canvas.textBoundary("").y;
	return style.paddingTop->get(textHeight, font.size)
		 + style.paddingBottom->get(textHeight, font.size)
		 + textHeight;
}

Geom::Rect drawLabel::alignText(const Geom::Size &textSize)
{
	Geom::Rect res;
	res.size = textSize;
	res.pos = contentRect.pos;

	res.pos.x = style.textAlign->combine<double>(
	    [&](int, const Styles::Text::TextAlign &align) -> double
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

Geom::Size drawLabel::getTextSize()
{
	Geom::Size res;

	res = canvas.textBoundary(text);

	overflows = res.x > contentRect.size.x;

	if (overflows) res.x = contentRect.size.x;

	return res;
}
