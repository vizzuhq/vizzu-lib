#ifndef DRAWINTERLACING_H
#define DRAWINTERLACING_H

#include "drawingcontext.h"
#include "guides.h"

namespace Vizzu
{
namespace Draw
{

class drawInterlacing : private DrawingContext
{
public:
	drawInterlacing(const DrawingContext &context,
	    const Guides &guides,
	    bool text);

private:
	const Guides &guides;

	void draw(bool horizontal, bool text);

	void draw(bool horizontal,
	    double stepSize,
	    double weight,
	    double rangeSize,
	    bool text);

	void drawDataLabel(bool horizontal,
	    const Geom::Point &stickPos,
	    double value,
	    const std::string &unit,
	    const Gfx::Color &textColor);

	void drawSticks(double stickIntensity,
	    bool horizontal,
	    const Geom::Point &stickPos);
};

}
}

#endif
