#ifndef DRAWGUIDES_H
#define DRAWGUIDES_H

#include "drawingcontext.h"

namespace Vizzu::Draw
{

class DrawGuides : private DrawingContext
{
public:
	DrawGuides(const DrawingContext &context);

private:
	void draw(bool horizontal);
	void
	drawGuide(bool horizontal, double val, const Gfx::Color &color);
};

}

#endif
