#ifndef CHART_RENDERING_DRAWMARKERINFO_H
#define CHART_RENDERING_DRAWMARKERINFO_H

#include "base/gfx/canvas.h"
#include "base/gfx/draw/textbox.h"
#include "chart/generator/plot.h"
#include "chart/main/layout.h"
#include "chart/main/style.h"
#include "painter/coordinatesystem.h"

#include "drawingcontext.h"

namespace Vizzu::Draw
{

class DrawMarkerInfo : public DrawingContext
{
	friend class MarkerDC;

public:
	using TextBox = Gfx::Draw::TextBox;
	using Content = const Gen::Plot::MarkerInfoContent;

	class MarkerDC
	{
	public:
		MarkerDC(const DrawMarkerInfo &parent,
		    Gfx::ICanvas &canvas,
		    const Geom::Rect &boundary,
		    Content &content);
		void draw(double weight);
		void
		interpolate(double weight1, MarkerDC &other, double weight2);

	protected:
		const DrawMarkerInfo &parent;
		Gfx::ICanvas &canvas;
		TextBox text;
		Geom::Point dataPoint;
		Geom::Point labelDir;
		Geom::Point arrow;
		Geom::Rect bubble;

		void loadMarker(Content &cnt);
		void fillTextBox(Content &cnt);
		void calculateLayout(Geom::Point hint = Geom::Point{0, 0});
	};

	void draw(Gfx::ICanvas &canvas, const Geom::Rect &boundary) const;

	const Styles::Tooltip &style;

private:
	void fadeInMarkerInfo(Gfx::ICanvas &canvas,
	    const Geom::Rect &boundary,
	    Content &cnt,
	    double weight) const;
	void fadeOutMarkerInfo(Gfx::ICanvas &canvas,
	    const Geom::Rect &boundary,
	    Content &cnt,
	    double weight) const;
	void moveMarkerInfo(Gfx::ICanvas &canvas,
	    const Geom::Rect &boundary,
	    Content &cnt1,
	    double weight1,
	    Content &cnt2,
	    double weight2) const;
};

}

#endif
