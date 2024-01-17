#ifndef DRAWORIENTEDLABEL_H
#define DRAWORIENTEDLABEL_H

#include "base/geom/transformedrect.h"
#include "chart/rendering/drawlabel.h"

#include "drawingcontext.h"

namespace Vizzu::Draw
{

struct OrientedLabel : DrawingContext
{
	void draw(Gfx::ICanvas &canvas,
	    const std::string &text,
	    const Geom::Line &labelPos,
	    const Styles::OrientedLabel &labelStyle,
	    double centered,
	    const Gfx::Color &textColor,
	    const Gfx::Color &bgColor,
	    Util::EventDispatcher::Event &event,
	    std::unique_ptr<Util::EventTarget> eventTarget) const;
};
}

#endif
