#include "drawlabel.h"

#include <memory>
#include <numbers>
#include <string>
#include <utility>

#include "base/geom/point.h"
#include "base/geom/rect.h"
#include "base/geom/transformedrect.h"
#include "base/gfx/canvas.h"
#include "base/gfx/font.h"
#include "base/util/eventdispatcher.h"
#include "chart/main/events.h"
#include "chart/main/style.h"

namespace Vizzu::Draw
{

void DrawLabel::draw(Gfx::ICanvas &canvas,
    const Geom::TransformedRect &fullRect,
    const std::string &text,
    const Styles::Label &style,
    Util::EventDispatcher::Event &onDraw,
    std::unique_ptr<Util::EventTarget> eventTarget,
    const Options &options) const
{
	auto relativeRect = Geom::Rect{{}, fullRect.size};

	auto font = Gfx::Font{style};
	auto paddedRect =
	    style.contentRect(relativeRect, font.size, options.flip);
	auto [alignRect, alignConstant] = alignText(paddedRect,
	    style,
	    Gfx::ICanvas::textBoundary(font, text));

	canvas.save();

	canvas.setFont(font);

	auto copyRect = fullRect;

	if (options.flip)
		copyRect.transform *= Geom::AffineTransform(fullRect.size,
		    1.0,
		    -std::numbers::pi);

	auto textTransform =
	    copyRect.transform
	    * Geom::AffineTransform(alignRect.bottomLeft());

	if (auto &&textColor = options.colorTransform(*style.color);
	    options.gradient)
		canvas.setBrushGradient(
		    options.gradient(textColor, textTransform.inverse()));
	else
		canvas.setBrushColor(textColor);

	if (onDraw.invoke(Events::OnTextDrawEvent{*eventTarget,
	        copyRect,
	        paddedRect,
	        alignConstant,
	        text})) {

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

	auto align = style.textAlign->combine<double>();
	if (auto space = paddedRect.size.x - textSize.x; space > 0)
		res.pos.x += space / 2.0 * (1 + align);

	return {res, align};
}

}