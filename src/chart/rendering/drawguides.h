#ifndef DRAWGUIDES_H
#define DRAWGUIDES_H

#include "drawaxes.h"
#include "drawingcontext.h"

namespace Vizzu::Draw
{

class DrawGuides
{
public:
	void draw(Gen::AxisId axisId,
	    const Math::Range<> &filter,
	    const Math::Range<> &otherFilter,
	    const Geom::AffineTransform &tr,
	    double w);

	const DrawAxes &parent;

private:
	void drawGuide(Gen::AxisId axisId,
	    double val,
	    const Math::Range<> &otherFilter,
	    const Geom::AffineTransform &tr,
	    const Gfx::Color &color);
};

}

#endif
