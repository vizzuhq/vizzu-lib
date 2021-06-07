#ifndef DRAWGUIDES_H
#define DRAWGUIDES_H

#include "drawingcontext.h"
#include "guides.h"

namespace Vizzu
{
namespace Draw
{

class drawGuides : private DrawingContext
{
public:
	drawGuides(const DrawingContext &context,
	    const Guides &guides);

private:
	const Guides &guides;

	void draw(bool horizontal);
	void drawGuide(bool horizontal, double val, const Gfx::Color &color);
};

}
}

#endif
