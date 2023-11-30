#ifndef CHART_LAYOUT_H
#define CHART_LAYOUT_H

#include "base/geom/rect.h"
#include "base/gfx/canvas.h"
#include "chart/generator/plot.h"

namespace Vizzu
{

class Layout
{
public:
	Layout() = default;

	Geom::Rect boundary;
	Geom::Rect title;
	Geom::Rect subtitle;
	Geom::Rect caption;
	Geom::Rect legend;
	Geom::Rect plot;
	Geom::Rect plotArea;
	Geom::Rect xTitle;
	Geom::Rect yTitle;
	Geom::Rect logo;

	void setBoundary(const Geom::Rect &boundary,
	    const Styles::Chart &style,
	    const Gen::PlotOptionsPtr &options,
	    Gfx::ICanvas &info);

	void setLogoBoundary(const Styles::Logo &logoStyle);
};

}

#endif
