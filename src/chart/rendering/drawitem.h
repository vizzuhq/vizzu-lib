#ifndef DRAWITEM_H
#define DRAWITEM_H

#include "chart/rendering/items/drawitem.h"

#include "drawingcontext.h"
#include "guides.h"

namespace Vizzu
{
namespace Draw
{

class drawItem : private DrawingContext
{
public:
	drawItem(const Diag::Marker &marker,
	    const DrawingContext &context);

	static bool mayDrawLines(const Guides &guides);
	void drawLines(const Guides &guides,
		const Styles::Guide &style,
		const Geom::Point &origo);
	void draw();
	void drawLabel();

private:
	const Diag::Marker &marker;

	bool shouldDraw();
	std::pair<Gfx::Color, Gfx::Color>
	getColor(const DrawItem &drawItem, double factor);
	void draw(const DrawItem &drawItem,
	    double factor,
	    bool line);
	void drawLabel(const DrawItem &drawItem);

	Gfx::Color getSelectedColor();
	std::string getLabelText();
};

}
}

#endif
