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
	drawItem(const Diag::Marker &item,
	    const DrawingContext &context);

	static bool mayDrawLines(const Guides &guides);
	void drawLines(const Guides &guides,
		const Styles::Guide &style,
		const Geom::Point &origo);
	void draw();

private:
	const Diag::Marker &item;

	bool shouldDraw();
	std::pair<Gfx::Color, Gfx::Color>
	getColor(const DrawItem &drawItem, double factor);
	void draw(const DrawItem &drawItem,
	    double factor,
	    bool line,
	    bool hasLabel = true);
	void drawLabel(const DrawItem &drawItem, const Gfx::Color &color);

	Gfx::Color getSelectedColor();
	std::string getLabelText();
};

}
}

#endif
