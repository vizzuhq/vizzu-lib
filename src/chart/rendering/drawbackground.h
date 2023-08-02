#ifndef CHART_RENDERING_DRAWBACKGROUND_H
#define CHART_RENDERING_DRAWBACKGROUND_H

#include "base/geom/rect.h"
#include "base/util/eventdispatcher.h"
#include "chart/main/events.h"
#include "chart/main/style.h"
#include "chart/rendering/drawingcontext.h"

namespace Vizzu::Draw
{

class DrawBackground : public DrawingContext
{
public:
	DrawBackground(
	    const DrawingContext &context,
	    const Geom::Rect &rect,
	    const Styles::Box &style,
	    const Util::EventDispatcher::event_ptr &onDraw =
	        Util::EventDispatcher::event_ptr(),
	    const Util::EventTarget &eventTarget = Util::EventTarget());
};

}

#endif
