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
	    RenderedChart &renderedChart) :
	    plot(plot),
	    coordSys(renderedChart.getCoordSys()),
	    canvas(canvas),
	    painter(*static_cast<Painter *>(canvas.getPainter())),
	    rootStyle(styles),
	    rootEvents(events),
	    layout(layout),
	    renderedChart(renderedChart)
	{
		if (plot) {
			auto plotArea = styles.plot.contentRect(layout.plot,
			    styles.calculatedSize());
			renderedChart =
			    RenderedChart{CoordinateSystem{plotArea,
			                      getOptions().angle,
			                      getOptions().coordSystem,
			                      plot->keepAspectRatio},
			        plot};
		}
		else {
			renderedChart = RenderedChart{
			    CoordinateSystem{layout.plotArea,
			        0.0,
			        ::Anim::Interpolated<Gen::CoordSystem>{
			            Gen::CoordSystem::cartesian},
			        Math::FuzzyBool()},
			    plot};
		}
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

	const Gen::Options &getOptions() const
	{
		return *plot->getOptions();
	}
};

}

#endif
