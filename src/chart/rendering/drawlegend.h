#ifndef CHART_RENDERING_DRAWLEGEND_H
#define CHART_RENDERING_DRAWLEGEND_H

#include "chart/rendering/drawingcontext.h"

namespace Vizzu::Draw
{

class DrawLegend : public DrawingContext
{
public:
	DrawLegend(const DrawingContext &context,
	    Gen::ChannelId channelType,
	    double weight);

private:
	Geom::Rect contentRect;
	const Events::Draw::Legend &events;
	const Styles::Legend &style;
	Gen::ChannelId type;
	double weight;
	double enabled{};
	double itemHeight;
	double titleHeight;

	void drawDimension(const Gen::DimensionAxis &axis);
	void drawMeasure(const Gen::MeasureAxis &axis);
	void drawMarker(Gfx::Color color, const Geom::Rect &rect);
	[[nodiscard]] Geom::Rect getItemRect(double index) const;
	[[nodiscard]] Geom::Rect getMarkerRect(
	    const Geom::Rect &itemRect) const;
	[[nodiscard]] Geom::Rect getLabelRect(
	    const Geom::Rect &itemRect) const;
	[[nodiscard]] Geom::Rect getBarRect() const;

	void drawTitle(const ::Anim::String &title);

	void extremaLabel(double value, int pos);
	void colorBar(const Geom::Rect &rect);
	void lightnessBar(const Geom::Rect &rect);
	void sizeBar(const Geom::Rect &rect);
};

}

#endif
