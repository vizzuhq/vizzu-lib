#ifndef DRAWINGCONTEXT_H
#define DRAWINGCONTEXT_H

#include "base/gfx/canvas.h"
#include "chart/generator/plot.h"
#include "chart/main/events.h"
#include "chart/main/layout.h"
#include "chart/main/style.h"
#include "painter/coordinatesystem.h"
#include "painter/painter.h"

#include "renderedchart.h"

namespace Vizzu::Draw
{

class DrawingContext
{
public:
	DrawingContext(Gfx::ICanvas &canvas,
	    const Layout &layout,
	    const Events &events,
	    const std::shared_ptr<Gen::Plot> &plot,
	    const Styles::Chart &styles,
	    const CoordinateSystem &coordSys,
	    RenderedChart &renderedChart) :
	    plot(plot),
	    coordSys(coordSys),
	    canvas(canvas),
	    painter(*static_cast<Painter *>(canvas.getPainter())),
	    rootStyle(styles),
	    rootEvents(events),
	    layout(layout),
	    renderedChart(renderedChart)
	{
		painter.setCoordSys(coordSys);
	}

	const std::shared_ptr<Gen::Plot> &plot;
	const CoordinateSystem &coordSys;
	Gfx::ICanvas &canvas;
	Painter &painter;
	const Styles::Chart &rootStyle;
	const Events &rootEvents;
	const Layout &layout;
	RenderedChart &renderedChart;

	const Gen::Options &getOptions() { return *plot->getOptions(); }
};

}

#endif
