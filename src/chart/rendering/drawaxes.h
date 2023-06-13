#ifndef DRAWAXES_H
#define DRAWAXES_H

#include "base/geom/line.h"

#include "drawingcontext.h"

namespace Vizzu
{
namespace Draw
{

class drawAxes : private DrawingContext
{
public:
	drawAxes(const DrawingContext &context);

	void drawBase();
	void drawLabels();

private:
	Geom::Line getAxis(Gen::ScaleId axisIndex) const;
	Geom::Point getTitleBasePos(Gen::ScaleId axisIndex,
	    int index) const;
	Geom::Point getTitleOffset(Gen::ScaleId axisIndex,
	    int index,
	    bool fades) const;
	void drawAxis(Gen::ScaleId axisIndex);
	void drawTitle(Gen::ScaleId axisIndex);
	void drawDimensionLabels(bool horizontal);
	void drawDimensionLabel(bool horizontal,
	    const Geom::Point &origo,
	    Gen::DimensionAxis::Values::const_iterator it);
};

}
}

#endif
