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
	Geom::Line getAxis(Diag::ChannelId axisIndex) const;
	Geom::Point getTitleBasePos(Diag::ChannelId axisIndex,
	    int index) const;
	Geom::Point getTitleOffset(Diag::ChannelId axisIndex,
	    int index,
	    bool fades) const;
	void drawAxis(Diag::ChannelId axisIndex);
	void drawTitle(Diag::ChannelId axisIndex);
	void drawDiscreteLabels(bool horizontal);
	void drawDiscreteLabel(bool horizontal,
	    const Geom::Point &origo,
	    Diag::DiscreteAxis::Values::const_iterator it);
};

}
}

#endif
