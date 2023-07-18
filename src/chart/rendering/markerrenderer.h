#ifndef MARKERRENDERER_H
#define MARKERRENDERER_H

#include "chart/rendering/markers/abstractmarker.h"

#include "drawingcontext.h"

namespace Vizzu
{
namespace Draw
{

class MarkerRenderer : private DrawingContext
{
public:
	MarkerRenderer(const Gen::Marker &marker,
	    const DrawingContext &context);
	void drawLines(const Styles::Guide &style,
	    const Geom::Point &origo);
	void draw();
	void drawLabel();

private:
	const Gen::Marker &marker;

	bool shouldDrawMarkerBody();
	std::pair<Gfx::Color, Gfx::Color> getColor(
	    const AbstractMarker &abstractMarker,
	    double factor,
	    bool label = false);
	void draw(const AbstractMarker &abstractMarker,
	    double factor,
	    bool line);
	void drawLabel(const AbstractMarker &abstractMarker,
	    size_t index);

	Gfx::Color getSelectedColor(bool label);
	std::string getLabelText(size_t index) const;
};

}
}

#endif
