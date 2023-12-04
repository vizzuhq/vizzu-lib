#ifndef MARKERRENDERER_H
#define MARKERRENDERER_H

#include "chart/rendering/markers/abstractmarker.h"

#include "drawingcontext.h"

namespace Vizzu::Draw
{

class MarkerRenderer : public DrawingContext
{
public:
	void drawLines() const;
	void drawMarkers() const;
	void drawLabels() const;

	Gfx::ICanvas &canvas;
	Painter &painter;

private:
	bool shouldDrawMarkerBody(const Gen::Marker &marker) const;
	std::pair<Gfx::Color, Gfx::Color> getColor(
	    const AbstractMarker &abstractMarker,
	    double factor,
	    bool label = false) const;
	void draw(const AbstractMarker &abstractMarker,
	    double factor,
	    bool line) const;
	void drawLabel(const AbstractMarker &abstractMarker,
	    size_t index) const;

	Gfx::Color getSelectedColor(const Gen::Marker &marker,
	    bool label) const;
	[[nodiscard]] std::string getLabelText(
	    const ::Anim::Interpolated<Gen::Marker::Label> &label,
	    size_t index) const;
};

}

#endif
