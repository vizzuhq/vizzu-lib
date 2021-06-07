#ifndef DRAWDIAGRAM_H
#define DRAWDIAGRAM_H

#include "drawingcontext.h"
#include "guides.h"

namespace Vizzu
{
namespace Draw
{

class DrawItem;

class drawDiagram : private DrawingContext
{
public:
	drawDiagram(const Geom::Rect &rect,
				const Diag::Diagram &diagram,
				Gfx::ICanvas &canvas,
				const DrawOptions &drawOptions,
				const Vizzu::Styles::Chart &style);

private:

	Guides guides;

	void drawItemGuides();
	void drawItems();
};

}
}

#endif
