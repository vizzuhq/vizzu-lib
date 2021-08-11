#ifndef DRAWAXES_H
#define DRAWAXES_H

#include "drawingcontext.h"
#include "guides.h"
#include "base/geom/line.h"

namespace Vizzu
{
namespace Draw
{

class drawAxes : private DrawingContext
{
public:
	drawAxes(const DrawingContext &context, const Guides &guides);

	void drawBase();
	void drawLabels();

private:
	const Guides &guides;

	Geom::Line getAxis(Diag::Scale::Type axisIndex) const;
	Geom::Point getTitleBasePos(Diag::Scale::Type axisIndex) const;
	Geom::Point getTitleOffset(Diag::Scale::Type axisIndex) const;
	void drawAxis(Diag::Scale::Type axisIndex,
	    const Gfx::Color &lineBaseColor);
	void drawTitle(Diag::Scale::Type axisIndex);
	void drawDiscreteLabels(bool horizontal);
	void drawDiscreteLabel(bool horizontal, 
		const Geom::Point &origo,
		Diag::DiscreteAxis::Values::const_iterator it);
};

}
}

#endif
