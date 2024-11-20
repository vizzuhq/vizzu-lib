#ifndef DRAWINTERLACING_H
#define DRAWINTERLACING_H

#include "drawaxes.h"
#include "drawingcontext.h"

namespace Vizzu::Draw
{

class DrawInterlacing
{
public:
	void drawGeometries() const;
	void drawTexts() const;

	const DrawAxes &parent;

private:
	void draw(Gen::AxisId axisIndex, bool text) const;

	void draw(const ::Anim::Interpolated<bool> &enabled,
	    Gen::AxisId axisIndex,
	    double stepSize,
	    double weight,
	    double rangeSize,
	    bool text) const;

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
