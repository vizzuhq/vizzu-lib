#ifndef CHART_RENDERING_DRAWBACKGROUND_H
#define CHART_RENDERING_DRAWBACKGROUND_H

#include "base/geom/rect.h"
#include "base/gfx/canvas.h"
#include "base/util/eventdispatcher.h"
#include "chart/main/events.h"
#include "chart/main/style.h"

namespace Vizzu
{
namespace Draw
{

class DrawBackground
{
public:
	DrawBackground(const Geom::Rect &rect,
	    Gfx::ICanvas &canvas,
	    const Styles::Box &style,
	    const Util::EventDispatcher::event_ptr &onDraw =
	        Util::EventDispatcher::event_ptr(),
	    const Util::EventTarget &eventTarget = Util::EventTarget());
};

}
}

#endif
