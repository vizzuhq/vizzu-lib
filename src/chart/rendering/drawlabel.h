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

	struct Options {
		Options(
			bool setColor = true,
			double alpha = 1.0,
			bool flip = false
		) : setColor(setColor), alpha(alpha), flip(flip) {}
		bool setColor;
		double alpha;
		bool flip;
	};

	drawLabel(const Geom::Rect &rect,
	    const std::string &text,
	    const Styles::Label &style,
	    const Util::EventDispatcher::event_ptr &onDraw,
	    Events::Events::OnTextDrawParam &&eventObj,
	    Gfx::ICanvas &canvas,
	    Options options = Options());

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
