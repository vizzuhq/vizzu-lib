#ifndef CHART_RENDERING_DRAWBACKGROUND_H
#define CHART_RENDERING_DRAWBACKGROUND_H

#include "base/geom/rect.h"
#include "base/util/eventdispatcher.h"
#include "chart/main/events.h"
#include "chart/main/style.h"
#include "chart/rendering/drawingcontext.h"

namespace Vizzu::Draw
{

struct DrawBackground : DrawingContext
{
	void draw(Gfx::ICanvas &canvas,
	    const Geom::Rect &rect,
	    const Styles::Box &style,
	    const Util::EventDispatcher::event_ptr &onDraw,
	    std::unique_ptr<Util::EventTarget> &&eventTarget) const;
};

}

#endif
