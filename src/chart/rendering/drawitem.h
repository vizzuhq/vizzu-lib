#ifndef DRAWITEM_H
#define DRAWITEM_H

#include "chart/rendering/items/drawitem.h"

#include "drawingcontext.h"

namespace Vizzu
{
namespace Draw
{

class drawItem : private DrawingContext
{
public:
	drawItem(const Gen::Marker &marker,
	    const DrawingContext &context);
	void drawLines(const Styles::Guide &style,
	    const Geom::Point &origo);
	void draw();
	void drawLabel();

private:
	const Gen::Marker &marker;

	bool shouldDrawMarkerBody();
	std::pair<Gfx::Color, Gfx::Color> getColor(
	    const DrawItem &drawItem,
	    double factor,
	    bool label = false);
	void draw(const DrawItem &drawItem, double factor, bool line);
	void drawLabel(const DrawItem &drawItem, size_t index);

	Gfx::Color getSelectedColor(bool label);
	std::string getLabelText(size_t index) const;
};

}
}

#endif
