#ifndef DRAWORIENTEDLABEL_H
#define DRAWORIENTEDLABEL_H

#include "base/geom/transformedrect.h"
#include "chart/rendering/drawlabel.h"

#include "drawingcontext.h"

namespace Vizzu::Draw
{

class OrientedLabel
{
public:
	const std::string &text;
	Geom::TransformedRect rect{};
	Geom::Rect contentRect{};

	[[nodiscard]] static OrientedLabel create(Gfx::ICanvas &canvas,
	    const std::string &text,
	    const Geom::Line &labelPos,
	    const Styles::OrientedLabel &labelStyle,
	    double centered);

	void draw(Gfx::ICanvas &canvas,
	    RenderedChart &renderedChart,
	    const Gfx::Color &textColor,
	    const Gfx::Color &bgColor,
	    Util::EventDispatcher::Event &event,
	    std::unique_ptr<Util::EventTarget> eventTarget) const;
};
}

#endif
