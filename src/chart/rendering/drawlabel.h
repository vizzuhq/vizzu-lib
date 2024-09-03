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
		std::optional<double> alpha{1.0};
		double bgAlpha{1.0};
		bool flip{false};
		std::function<void(Gfx::ICanvas &,
		    const Geom::AffineTransform &,
		    const Gfx::Color &)>
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
