#ifndef DRAWGUIDES_H
#define DRAWGUIDES_H

#include "drawingcontext.h"

namespace Vizzu
{
namespace Draw
{

class drawGuides : private DrawingContext
{
public:
	drawGuides(const DrawingContext &context);

private:
	void draw(bool horizontal);
	void
	drawGuide(bool horizontal, double val, const Gfx::Color &color);
};

}
}

#endif
