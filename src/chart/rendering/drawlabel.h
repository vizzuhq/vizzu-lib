#ifndef CHART_RENDERING_DRAWLABEL_H
#define CHART_RENDERING_DRAWLABEL_H

#include "base/geom/rect.h"
#include "base/gfx/canvas.h"
#include "base/util/eventdispatcher.h"
#include "chart/main/style.h"
#include "chart/main/events.h"

namespace Vizzu
{
namespace Draw
{

class drawLabel
{
public:

	drawLabel(const Geom::Rect &rect,
	    const std::string &text,
	    const Styles::Label &style,
	    const Util::EventDispatcher::event_ptr &onDraw,
	    Events::Events::OnTextDrawParam &&eventObj,
	    Gfx::ICanvas &canvas,
	    bool setColor = true,
	    double alpha = 1.0);

	static double getHeight(const Styles::Label &style,
	    Gfx::ICanvas &canvas);

private:
	Geom::Rect contentRect;
	std::string text;
	const Styles::Label &style;
	const Util::EventDispatcher::event_ptr &onDraw;
	Gfx::ICanvas &canvas;
	bool overflows;
	Geom::Rect alignText(const Geom::Size &textSize);
	Geom::Size getTextSize();
};

}
}

#endif
