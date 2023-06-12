#ifndef CHART_RENDERING_DRAWLEGEND_H
#define CHART_RENDERING_DRAWLEGEND_H

#include "base/geom/rect.h"
#include "base/gfx/canvas.h"
#include "chart/generator/diagram.h"
#include "chart/main/events.h"
#include "chart/main/style.h"

namespace Vizzu
{
namespace Draw
{

class drawLegend
{
public:
	drawLegend(const Geom::Rect &rect,
	    const Diag::Diagram &diagram,
	    const Events::Draw::Legend &events,
	    Gfx::ICanvas &canvas,
	    Diag::ScaleId scaleType,
	    double weight);

private:
	Geom::Rect contentRect;
	const Diag::Diagram &diagram;
	const Events::Draw::Legend &events;
	Gfx::ICanvas &canvas;
	Diag::ScaleId type;
	double weight;
	double enabled;
	const Styles::Legend &style;
	double itemHeight;
	double titleHeight;

	void drawDiscrete(const Diag::DiscreteAxis &axis);
	void drawContinous(const Diag::Axis &axis);
	void drawMarker(Gfx::Color color, const Geom::Rect &rect);
	Geom::Rect getItemRect(double index) const;
	Geom::Rect getMarkerRect(const Geom::Rect &itemRect) const;
	Geom::Rect getLabelRect(const Geom::Rect &itemRect) const;
	Geom::Rect getBarRect() const;

	void drawTitle(const ::Anim::String &title);

	void extremaLabel(double value, int pos);
	void colorBar(const Geom::Rect &rect);
	void lightnessBar(const Geom::Rect &rect);
	void sizeBar(const Geom::Rect &rect);
};

}
}

#endif
