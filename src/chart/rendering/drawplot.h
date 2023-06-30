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
	explicit drawPlot(const DrawingContext &context);

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
