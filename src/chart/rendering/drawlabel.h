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
		explicit Options(bool setColor = true,
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

	void draw(Gfx::ICanvas &canvas,
	    const Geom::TransformedRect &rect,
	    const std::string &text,
	    const Styles::Label &style,
	    Util::EventDispatcher::Event &onDraw,
	    std::unique_ptr<Util::EventTarget> eventTarget,
	    Options options = Options()) const;

	static double getHeight(const Styles::Label &style,
	    Gfx::ICanvas &canvas);

private:
	[[nodiscard]] static std::pair<Geom::Rect, double> alignText(
	    const Geom::Rect &contentRect,
	    const Styles::Label &style,
	    const Geom::Size &textSize);
};

}

#endif
