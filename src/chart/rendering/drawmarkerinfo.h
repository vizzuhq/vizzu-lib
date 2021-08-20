#ifndef CHART_RENDERING_DRAWMARKERINFO_H
#define CHART_RENDERING_DRAWMARKERINFO_H

#include "base/gfx/canvas.h"
#include "base/gfx/draw/textbox.h"
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
	drawMarkerInfo(Geom::Rect plot, Gfx::ICanvas &canvas,
		const Diag::Diagram &diagram, const Styles::MarkerInfo &style);

private:
	typedef Gfx::Draw::TextBox TextBox;
	typedef const Diag::Diagram::MarkerInfoContent Content;

	const Diag::Diagram &diagram;
	Draw::CoordinateSystem* coordSystem;
	Gfx::ICanvas &canvas;
	const Styles::MarkerInfo &style;
	const Geom::Rect plot;

	Geom::Line getDataPoint(Content& cnt);
	Geom::Point calculatePosition(const Geom::Line& control, const Geom::Size& size);

	void fadeInMarkerInfo(Content& cnt, double weight);
	void fadeOutMarkerInfo(Content& cnt, double weight);
	void moveMarkerInfo(Content& cnt1, double weight1, Content& cnt2, double weight2);

	void draw(const Geom::Point& position, TextBox& text, double weight);
	void fillTextBox(Content& cnt, TextBox& text);
	void highlightDataPoint(const Geom::Point& position, double weight);
};

}
}

#endif //CHART_RENDERING_DRAWMARKERINFO_H
