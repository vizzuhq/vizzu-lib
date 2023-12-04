#ifndef DRAW_PLOT_H
#define DRAW_PLOT_H

#include "drawingcontext.h"
#include "markerrenderer.h"

namespace Vizzu::Draw
{

class AbstractMarker;

class DrawPlot : public DrawingContext
{
public:
	void draw(Gfx::ICanvas &canvas,
	    Painter &painter,
	    const Geom::Rect &plotRect) const;

private:
	void drawPlotArea(Gfx::ICanvas &canvas,
	    Painter &painter,
	    bool clip) const;
};

}

#endif
