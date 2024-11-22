#ifndef DRAWGUIDES_H
#define DRAWGUIDES_H

#include "drawaxes.h"
#include "drawingcontext.h"

namespace Vizzu::Draw
{

class DrawGuides
{
public:
	void draw(Gen::AxisId axisId);

	const DrawAxes &parent;

private:
	void drawGuide(Gen::AxisId axisId,
	    double val,
	    const Gfx::Color &color);
};

}

#endif
