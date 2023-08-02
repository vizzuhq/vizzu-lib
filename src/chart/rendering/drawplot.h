#ifndef DRAW_PLOT_H
#define DRAW_PLOT_H

#include "drawingcontext.h"

namespace Vizzu::Draw
{

class AbstractMarker;

class DrawPlot : private DrawingContext
{
public:
	explicit DrawPlot(const DrawingContext &context);

private:
	void drawArea(bool clip);
	void clipPlotArea();
	void drawMarkerGuides();
	void drawMarkers();
	void drawMarkerLabels();
};

}

#endif
