#ifndef DRAWDIAGRAM_H
#define DRAWDIAGRAM_H

#include "drawingcontext.h"

namespace Vizzu
{
namespace Draw
{

class DrawItem;

class drawDiagram : private DrawingContext
{
public:
	drawDiagram(const Geom::Rect &rect,
				const Diag::Diagram &diagram,
				Gfx::ICanvas &canvas,
				const DrawOptions &drawOptions,
				const Vizzu::Styles::Chart &style,
				const Events::Draw &events);

private:

	void clipPlotArea();
	void drawMarkerGuides();
	void drawMarkers();
	void drawMarkerLabels();
};

}
}

#endif
