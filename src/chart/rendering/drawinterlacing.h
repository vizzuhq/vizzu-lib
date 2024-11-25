#ifndef DRAWINTERLACING_H
#define DRAWINTERLACING_H

#include "drawaxes.h"
#include "drawingcontext.h"

namespace Vizzu::Draw
{

class DrawInterlacing
{
public:
	void drawGeometries(Gen::AxisId axisIndex) const;
	void drawTexts(Gen::AxisId axisIndex) const;

	const DrawAxes &parent;

private:
	void drawInterlacing(Gen::AxisId axisIndex,
	    const Gfx::Color &interlacingColor,
	    const Geom::Rect &rect) const;

	void drawDataLabel(const ::Anim::Interpolated<bool> &enabled,
	    Gen::AxisId axisIndex,
	    const Geom::Point &tickPos,
	    double value,
	    const ::Anim::String &unit,
	    double alpha) const;

	void drawSticks(double tickLength,
	    const Gfx::Color &tickColor,
	    Gen::AxisId axisIndex,
	    const Geom::Point &tickPos) const;
};

}

#endif
