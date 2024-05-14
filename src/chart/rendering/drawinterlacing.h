#ifndef DRAWINTERLACING_H
#define DRAWINTERLACING_H

#include "drawingcontext.h"

namespace Vizzu::Draw
{

class DrawInterlacing : public DrawingContext
{
public:
	void drawGeometries() const;
	void drawTexts() const;

	Gfx::ICanvas &canvas;
	Painter &painter;

private:
	void draw(bool horizontal, bool text) const;

	void draw(const ::Anim::Interpolated<bool> &enabled,
	    bool horizontal,
	    double stepSize,
	    double weight,
	    double rangeSize,
	    bool text) const;

	void drawDataLabel(const ::Anim::Interpolated<bool> &enabled,
	    bool horizontal,
	    const Geom::Point &tickPos,
	    double value,
	    const ::Anim::String &unit,
	    double alpha) const;

	void drawSticks(double tickIntensity,
	    bool horizontal,
	    const Geom::Point &tickPos) const;
};

}

#endif
