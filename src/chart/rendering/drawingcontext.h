#ifndef DRAWINGCONTEXT_H
#define DRAWINGCONTEXT_H

#include "base/gfx/canvas.h"
#include "chart/generator/plot.h"
#include "chart/main/events.h"
#include "chart/main/layout.h"
#include "chart/main/style.h"
#include "painter/coordinatesystem.h"
#include "painter/painter.h"

namespace Vizzu::Draw
{

class DrawingContext
{
public:
	DrawingContext(
	    Gfx::ICanvas &canvas,
	    const Layout &layout,
	    const Events::Draw &events,
	    const Gen::Plot &plot) :
	    plot(plot),
	    canvas(canvas),
	    painter(*static_cast<Painter *>(canvas.getPainter())),
	    options(*plot.getOptions()),
	    rootStyle(plot.getStyle()),
	    rootEvents(events),
	    layout(layout)
	{
		auto plotArea = rootStyle.plot.contentRect
			(layout.plot, rootStyle.calculatedSize());
		
		coordSys = CoordinateSystem(
		    plotArea,
		    options.angle,
		    options.coordSystem,
		    plot.keepAspectRatio
		);

		painter.setCoordSys(coordSys);
	}

	const Gen::Plot &plot;
	CoordinateSystem coordSys;
	Gfx::ICanvas &canvas;
	Painter &painter;
	const Gen::Options &options;
	const Styles::Chart &rootStyle;
	const Events::Draw &rootEvents;
	const Layout &layout;
};

}

#endif
