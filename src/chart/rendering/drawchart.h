
#ifndef VIZZU_DRAWCHART_H
#define VIZZU_DRAWCHART_H

#include "drawingcontext.h"

namespace Vizzu::Draw
{

class DrawChart : public DrawingContext
{
	void drawBackground(Gfx::ICanvas &canvas,
	    const Geom::Rect &bounds) const;

	void drawPlot(Gfx::ICanvas &canvas,
	    Painter &painter,
	    const Geom::Rect &plotRect) const;

	void drawLegend(Gfx::ICanvas &canvas,
	    const Geom::Rect &bounds) const;

	template <auto targetGetter, class MemberGetter>
	void drawHeading(Gfx::ICanvas &canvas,
	    const Layout &layout,
	    const MemberGetter &&getter) const;

	void drawMarkerInfo(Gfx::ICanvas &canvas,
	    const Geom::Rect &bounds) const;

	void drawLogo(Gfx::ICanvas &canvas,
	    const Geom::Rect &bounds) const;

public:
	void draw(Gfx::ICanvas &canvas,
	    const Layout &layout,
	    const Gfx::PathSampler::Options &options) const;
};

}

#endif
