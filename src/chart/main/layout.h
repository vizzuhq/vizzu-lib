#ifndef CHART_LAYOUT_H
#define CHART_LAYOUT_H

#include "base/geom/rect.h"
#include "base/gfx/canvas.h"

#include "chart/generator/diagram.h"

namespace Vizzu
{

class Layout
{
public:
	Layout() {}

	Geom::Rect boundary;
	Geom::Rect title;
	Geom::Rect legend;
	Geom::Rect plot;
	Geom::Rect plotArea;
	Geom::Rect xTitle;
	Geom::Rect yTitle;

	void setBoundary(
		const Geom::Rect &boundary,
		const Diag::Diagram &diagram,
		Gfx::ICanvas &info);

	void setBoundary(
		const Geom::Rect &boundary,
		Gfx::ICanvas &info);

	const std::string getElementNameAt(const Geom::Point &point) const;
};

}

#endif
