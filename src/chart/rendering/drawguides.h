#ifndef DRAWGUIDES_H
#define DRAWGUIDES_H

#include "drawaxes.h"
#include "drawingcontext.h"

namespace Vizzu::Draw
{

class DrawGuides
{
public:
	void draw();

	const DrawAxes &parent;

private:
	void draw(Gen::AxisId axisId);
	void drawGuide(Gen::AxisId axisId,
	    double val,
	    const Gfx::Color &color);
};

}

#endif
