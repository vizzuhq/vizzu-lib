#include "drawlabel.h"

namespace Vizzu::Draw
{

void DrawLabel::draw(Gfx::ICanvas &canvas,
    const Geom::TransformedRect &fullRect,
    const std::string &text,
    const Styles::Label &style,
    Util::EventDispatcher::Event &onDraw,
    std::unique_ptr<Util::EventTarget> eventTarget,
    Options options) const
{
	auto relativeRect = Geom::Rect{{}, fullRect.size};

	if (!style.backgroundColor->isTransparent()) {
		canvas.save();
		auto bgColor = *style.backgroundColor * options.bgAlpha;
		canvas.setBrushColor(bgColor);
		canvas.setLineColor(bgColor);
		canvas.transform(fullRect.transform);
		canvas.rectangle(relativeRect);
		canvas.restore();
	}

	auto font = Gfx::Font{style};
	auto paddedRect = style.contentRect(relativeRect, font.size);
	auto [alignRect, alignConstant] = alignText(paddedRect,
	    style,
	    Gfx::ICanvas::textBoundary(font, text));

	canvas.save();

	canvas.setFont(font);
	if (options.alpha)
		canvas.setTextColor(*style.color * *options.alpha);

	auto copyRect = fullRect;

	if (options.flip)
		copyRect.transform *=
		    Geom::AffineTransform(fullRect.size, 1.0, -M_PI);

	if (onDraw.invoke(Events::OnTextDrawEvent{*eventTarget,
	        copyRect,
	        paddedRect,
	        alignConstant,
	        text})) {

		auto textTransform =
		    fullRect.transform
		    * Geom::AffineTransform(alignRect.bottomLeft());

		if (options.flip)
			textTransform *=
			    Geom::AffineTransform(alignRect.size, 1.0, -M_PI);

		canvas.transform(textTransform);

		canvas.text({{}, alignRect.size}, text);

		renderedChart.emplace(copyRect, std::move(eventTarget));
	}

	canvas.restore();
}

std::pair<Geom::Rect, double> DrawLabel::alignText(
    const Geom::Rect &paddedRect,
    const Styles::Label &style,
    const Geom::Size &textSize)
{
	Geom::Rect res{paddedRect.pos, textSize};

	auto align = style.textAlign->calculate<double>();
	if (auto space = paddedRect.size.x - textSize.x; space > 0)
		res.pos.x += space / 2.0 * (1 + align);

	return {res, align};
}

}