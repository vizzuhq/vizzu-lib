#ifndef DRAWINGCONTEXT_H
#define DRAWINGCONTEXT_H

#include "base/gfx/canvas.h"
#include "chart/generator/plot.h"
#include "chart/main/events.h"
#include "chart/main/style.h"
#include "painter/coordinatesystem.h"
#include "painter/painter.h"

#include "drawoptions.h"

namespace Vizzu
{
namespace Draw
{

class DrawingContext
{
public:
	DrawingContext(const Geom::Rect &rect,
	    const Diag::Plot &plot,
	    Gfx::ICanvas &canvas,
	    const DrawOptions &drawOptions,
	    const Styles::Chart &style,
	    const Events::Draw &events) :
	    drawOptions(drawOptions),
	    plot(plot),
	    canvas(canvas),
	    painter(dynamic_cast<IPainter &>(canvas)),
	    options(*plot.getOptions()),
	    style(style),
	    events(events),
	    boundingRect(rect)
	{
		coordSys = CoordinateSystem(
		    style.plot.contentRect(rect, style.calculatedSize()),
		    options.angle.get(),
		    options.polar.get(),
		    plot.keepAspectRatio);

		painter.setCoordSys(coordSys);
	}

	const DrawOptions &drawOptions;
	const Diag::Plot &plot;
	CoordinateSystem coordSys;
	Gfx::ICanvas &canvas;
	IPainter &painter;
	const Diag::Options &options;
	const Styles::Chart &style;
	const Events::Draw &events;
	Geom::Rect boundingRect;
};

}
}

#endif
