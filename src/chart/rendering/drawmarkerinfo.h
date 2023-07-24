#ifndef CHART_RENDERING_DRAWMARKERINFO_H
#define CHART_RENDERING_DRAWMARKERINFO_H

#include "base/gfx/canvas.h"
#include "base/gfx/draw/textbox.h"
#include "chart/generator/plot.h"
#include "chart/main/layout.h"
#include "chart/main/style.h"
#include "painter/coordinatesystem.h"

namespace Vizzu
{
namespace Draw
{

class DrawMarkerInfo
{
	friend class MarkerDC;

public:
	typedef Gfx::Draw::TextBox TextBox;
	typedef const Gen::Plot::MarkerInfoContent Content;

	class MarkerDC
	{
	public:
		MarkerDC(DrawMarkerInfo &parent, Content &cnt);
		void draw(double weight);
		void
		interpolate(double weight1, MarkerDC &other, double weight2);

	protected:
		DrawMarkerInfo &parent;
		TextBox text;
		Geom::Point dataPoint;
		Geom::Point labelDir;
		Geom::Point arrow;
		Geom::Rect bubble;

		void loadMarker(Content &cnt);
		void fillTextBox(Content &cnt);
		void calculateLayout(Geom::Point hint = Geom::Point{0, 0});
	};

public:
	DrawMarkerInfo(const Layout &layout,
	    Gfx::ICanvas &canvas,
	    const Gen::Plot &plot);

private:
	const Layout &layout;
	Gfx::ICanvas &canvas;
	const Gen::Plot &plot;
	Draw::CoordinateSystem *coordSystem;
	const Styles::Tooltip &style;

	void fadeInMarkerInfo(Content &cnt, double weight);
	void fadeOutMarkerInfo(Content &cnt, double weight);
	void moveMarkerInfo(Content &cnt1,
	    double weight1,
	    Content &cnt2,
	    double weight2);
};

}
}

#endif
