#ifndef CHART_RENDERING_DRAWMARKERINFO_H
#define CHART_RENDERING_DRAWMARKERINFO_H

#include "base/gfx/canvas.h"
#include "chart/generator/diagram.h"
#include "chart/main/style.h"
#include "painter/coordinatesystem.h"

namespace Vizzu
{
namespace Draw
{

class drawMarkerInfo
{
public:
	drawMarkerInfo(Geom::Rect plot, Gfx::ICanvas &canvas, const Diag::Diagram &diagram);

private:
	typedef const Diag::Diagram::MarkerInfoContent Content;

	const Diag::Diagram &diagram;
	Draw::CoordinateSystem* coordSystem;
	Gfx::ICanvas &canvas;
	double lineHeight;

	Geom::Line getDataPoint(Content& cnt);
	Geom::Point calculatePosition(Geom::Line& control);
	Geom::Size calculateTextSize(Content& cnt);

	void fadeInMarkerInfo(Content& cnt, double weight);
	void fadeOutMarkerInfo(Content& cnt, double weight);
	void moveMarkerInfo(Content& cnt1, double weight1, Content& cnt2, double weight2);

	void draw(Geom::Point& position, Content& cnt, double weight);
	void drawBackground(Geom::Rect& rect, double radius, double weight);
	void drawText(Geom::Point& position, Content& cnt, double weight);
};

}
}

#endif //CHART_RENDERING_DRAWMARKERINFO_H
