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
			const auto &options = *plot->getOptions();
			renderedChart = RenderedChart{{plotArea,
			                                  options.angle,
			                                  options.coordSystem,
			                                  plot->keepAspectRatio},
			    plot};
		}
		else {
			renderedChart = RenderedChart{
			    {layout.plotArea,
			        0.0,
			        ::Anim::Interpolated<Gen::CoordSystem>{
			            Gen::CoordSystem::cartesian},
			        Math::FuzzyBool()},
			    plot};
		}
		painter.setCoordSys(renderedChart.getCoordSys());
	}

	const std::shared_ptr<Gen::Plot> &plot;
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
