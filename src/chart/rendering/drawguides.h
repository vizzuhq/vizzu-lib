#ifndef DRAWGUIDES_H
#define DRAWGUIDES_H

#include "drawingcontext.h"

namespace Vizzu::Draw
{

class DrawGuides : public DrawingContext
{
public:
	void draw();

	Gfx::ICanvas &canvas;
	Painter &painter;

private:
	void draw(Gen::AxisId axisId);
	void drawGuide(Gen::AxisId axisId,
	    double val,
	    const Gfx::Color &color);
};

}

#endif
