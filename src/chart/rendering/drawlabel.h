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
		Gfx::ColorTransform colorTransform{};
		bool flip{false};
		std::function<const Gfx::LinearGradient &(const Gfx::Color &,
		    const Geom::AffineTransform &)>
		    gradient{};
	};

	void draw(Gfx::ICanvas &canvas,
	    const Geom::TransformedRect &fullRect,
	    const std::string &text,
	    const Styles::Label &style,
	    Util::EventDispatcher::Event &onDraw,
	    std::unique_ptr<Util::EventTarget> eventTarget,
	    Options options) const;

private:
	[[nodiscard]] static std::pair<Geom::Rect, double> alignText(
	    const Geom::Rect &paddedRect,
	    const Styles::Label &style,
	    const Geom::Size &textSize);
};

}

#endif
