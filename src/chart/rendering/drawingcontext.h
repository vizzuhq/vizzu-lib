#ifndef DRAWINGCONTEXT_H
#define DRAWINGCONTEXT_H

#include "base/gfx/canvas.h"
#include "chart/generator/plot.h"
#include "chart/main/events.h"
#include "chart/main/style.h"
#include "painter/coordinatesystem.h"
#include "painter/painter.h"

namespace Vizzu
{
namespace Draw
{

class DrawingContext
{
public:
	DrawingContext(const Geom::Rect &rect,
	    const Gen::Plot &plot,
	    Gfx::ICanvas &canvas,
	    const Styles::Chart &style,
	    const Events::Draw &events) :
	    plot(plot),
	    canvas(canvas),
	    painter(*static_cast<Painter *>(canvas.getPainter())),
	    options(*plot.getOptions()),
	    style(style),
	    events(events),
	    boundingRect(rect)
	{
		coordSys = CoordinateSystem(
		    style.plot.contentRect(rect, style.calculatedSize()),
		    options.angle,
		    options.polar,
		    plot.keepAspectRatio);

		painter.setCoordSys(coordSys);
	}

	const Gen::Plot &plot;
	CoordinateSystem coordSys;
	Gfx::ICanvas &canvas;
	Painter &painter;
	const Gen::Options &options;
	const Styles::Chart &style;
	const Events::Draw &events;
	Geom::Rect boundingRect;
};

}
}

#endif
