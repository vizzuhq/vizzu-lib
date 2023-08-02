#ifndef DRAWINTERLACING_H
#define DRAWINTERLACING_H

#include "drawingcontext.h"

namespace Vizzu::Draw
{

class DrawInterlacing : private DrawingContext
{
public:
	DrawInterlacing(const DrawingContext &context, bool text);

private:
	void draw(bool horizontal, bool text);

	void draw(const ::Anim::Interpolated<bool> &enabled,
	    bool horizontal,
	    double stepSize,
	    double weight,
	    double rangeSize,
	    bool text);

	void drawDataLabel(const ::Anim::Interpolated<bool> &enabled,
	    bool horizontal,
	    const Geom::Point &tickPos,
	    double value,
	    const std::string &unit,
	    const Gfx::Color &textColor);

	void drawSticks(double tickIntensity,
	    bool horizontal,
	    const Geom::Point &tickPos);
};

}

#endif
