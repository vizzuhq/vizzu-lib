#ifndef DRAW_PLOT_H
#define DRAW_PLOT_H

#include "drawingcontext.h"

namespace Vizzu
{
namespace Draw
{

class DrawItem;

class drawPlot : private DrawingContext
{
public:
	drawPlot(const Geom::Rect &rect,
	    const Gen::Plot &plot,
	    Gfx::ICanvas &canvas,
	    const Vizzu::Styles::Chart &style,
	    const Events::Draw &events);

private:
	void drawArea(bool clip);
	void clipPlotArea();
	void drawMarkerGuides();
	void drawMarkers();
	void drawMarkerLabels();
};

}
}

#endif
