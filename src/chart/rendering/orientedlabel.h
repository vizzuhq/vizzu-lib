#ifndef DRAWORIENTEDLABEL_H
#define DRAWORIENTEDLABEL_H

#include <optional>

#include "base/geom/transformedrect.h"
#include "chart/rendering/drawlabel.h"

#include "drawingcontext.h"

namespace Vizzu::Draw
{

struct OrientedLabel : DrawLabel
{
	void draw(Gfx::ICanvas &canvas,
	    const std::string &text,
	    const Geom::Line &labelPos,
	    const Styles::OrientedLabel &labelStyle,
	    double centered,
	    const Gfx::ColorTransform &colorTransform,
	    Util::EventDispatcher::Event &event,
	    std::unique_ptr<Util::EventTarget> eventTarget) const;
};
}

#endif
