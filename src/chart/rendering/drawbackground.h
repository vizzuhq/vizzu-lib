#ifndef CHART_RENDERING_DRAWBACKGROUND_H
#define CHART_RENDERING_DRAWBACKGROUND_H

#include "base/geom/rect.h"
#include "base/gfx/canvas.h"
#include "base/util/eventdispatcher.h"
#include "chart/main/style.h"

namespace Vizzu
{
namespace Draw
{

class drawBackground
{
public:
	struct OnDrawParam : public Util::EventDispatcher::Params
	{
		Geom::Rect rect;
		OnDrawParam(Geom::Rect rect) : rect(rect) {}
		std::string dataToJson() const override {
			return "{\"rect\":" + std::string(rect) + "}";
		}
	};

	drawBackground(const Geom::Rect &rect,
	    Gfx::ICanvas &canvas,
	    const Styles::Box &style,
		const Util::EventDispatcher::event_ptr &onDraw
			= Util::EventDispatcher::event_ptr());
};

}
}

#endif
