#ifndef DRAWINGCONTEXT_H
#define DRAWINGCONTEXT_H

#include "base/gfx/canvas.h"
#include "chart/generator/plot.h"
#include "chart/main/events.h"
#include "chart/main/style.h"
#include "chart/main/layout.h"
#include "painter/coordinatesystem.h"
#include "painter/painter.h"

namespace Vizzu
{
namespace Draw
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
	    style(plot.getStyle()),
	    events(events),
		layout(layout)
	{
		auto plotArea = style.plot.contentRect
			(layout.plot, style.calculatedSize());
		
		coordSys = CoordinateSystem(
		    plotArea,
		    options.angle,
		    options.polar,
		    plot.keepAspectRatio
		);

		painter.setCoordSys(coordSys);
	}

	const Gen::Plot &plot;
	CoordinateSystem coordSys;
	Gfx::ICanvas &canvas;
	Painter &painter;
	const Gen::Options &options;
	const Styles::Chart &style;
	const Events::Draw &events;
	const Layout &layout;
};

}
}

#endif
