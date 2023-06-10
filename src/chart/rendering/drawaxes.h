#ifndef DRAWAXES_H
#define DRAWAXES_H

#include "drawingcontext.h"
#include "base/geom/line.h"

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
	Geom::Line getAxis(Diag::ScaleId axisIndex) const;
	Geom::Point getTitleBasePos(Diag::ScaleId axisIndex, int index) const;
	Geom::Point getTitleOffset(Diag::ScaleId axisIndex, int index, bool fades) const;
	void drawAxis(Diag::ScaleId axisIndex);
	void drawTitle(Diag::ScaleId axisIndex);
	void drawDiscreteLabels(bool horizontal);
	void drawDiscreteLabel(bool horizontal, 
		const Geom::Point &origo,
		Diag::DiscreteAxis::Values::const_iterator it);
};

}
}

#endif
