#ifndef MARKERRENDERER_H
#define MARKERRENDERER_H

#include "chart/rendering/markers/abstractmarker.h"

#include "drawingcontext.h"

namespace Vizzu::Draw
{

class MarkerRenderer : public DrawingContext
{
public:
	static MarkerRenderer create(const DrawingContext &ctx);

	void drawLines(Gfx::ICanvas &canvas, Painter &painter) const;
	void drawMarkers(Gfx::ICanvas &canvas, Painter &painter) const;
	void drawLabels(Gfx::ICanvas &canvas) const;

	std::vector<AbstractMarker> markers;

private:
	[[nodiscard]] static bool shouldDrawMarkerBody(
	    const Gen::Marker &marker);
	[[nodiscard]] std::pair<Gfx::Color, Gfx::Color> getColor(
	    const AbstractMarker &abstractMarker,
	    bool label = false) const;
	void draw(Gfx::ICanvas &canvas,
	    Painter &painter,
	    const AbstractMarker &abstractMarker,
	    double factor,
	    bool isLine) const;
	void drawLabel(Gfx::ICanvas &canvas,
	    const AbstractMarker &abstractMarker,
	    const std::string &unit,
	    bool keepMeasure,
	    ::Anim::InterpolateIndex index) const;

	[[nodiscard]] Gfx::Color
	getSelectedColor(const Gen::Marker &marker, bool label) const;
	[[nodiscard]] std::string getLabelText(
	    const ::Anim::Interpolated<Gen::Marker::Label> &label,
	    const std::string &unit,
	    bool keepMeasure,
	    ::Anim::InterpolateIndex index) const;
};

}

#endif
