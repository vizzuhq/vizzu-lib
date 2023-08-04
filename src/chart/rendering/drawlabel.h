#ifndef CHART_RENDERING_DRAWLABEL_H
#define CHART_RENDERING_DRAWLABEL_H

#include "base/geom/transformedrect.h"
#include "base/gfx/canvas.h"
#include "base/util/eventdispatcher.h"
#include "chart/main/events.h"
#include "chart/main/style.h"
#include "chart/rendering/drawingcontext.h"

namespace Vizzu::Draw
{

class DrawLabel : public DrawingContext
{
public:
	struct Options
	{
		Options(bool setColor = true,
		    double alpha = 1.0,
		    bool flip = false) :
		    setColor(setColor),
		    alpha(alpha),
		    flip(flip)
		{}
		bool setColor;
		double alpha;
		bool flip;
	};

	DrawLabel(const DrawingContext &context,
	    const Geom::TransformedRect &rect,
	    const std::string &text,
	    const Styles::Label &style,
	    const Util::EventDispatcher::event_ptr &onDraw,
	    const Util::EventTarget &eventTarget,
	    Options options = Options());

	static double getHeight(const Styles::Label &style,
	    Gfx::ICanvas &canvas);

private:
	Geom::Rect contentRect;
	const Styles::Label &style;
	const Util::EventDispatcher::event_ptr &onDraw;
	Geom::Rect alignText(const Geom::Size &textSize);
};

}

#endif
