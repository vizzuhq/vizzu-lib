#ifndef DRAWINGCONTEXT_H
#define DRAWINGCONTEXT_H

#include "base/gfx/canvas.h"
#include "chart/generator/plotptr.h"
#include "chart/main/events.h"
#include "chart/main/layout.h"
#include "chart/main/style.h"
#include "painter/coordinatesystem.h"
#include "painter/painter.h"

#include "renderedchart.h"

namespace Vizzu::Draw
{

struct DrawingContext
{
	const Gen::PlotPtr &plot;
	const Gen::Options *options;
	RenderedChart &renderedChart;
	const CoordinateSystem &coordSys;
	const Styles::Chart &rootStyle;
	const Events &rootEvents;

	[[nodiscard]] const Gen::Options &getOptions() const
	{
		return *options;
	}

	[[nodiscard]] const DrawingContext &ctx() const { return *this; }
};

}

#endif
