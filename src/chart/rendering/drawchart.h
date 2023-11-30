
#ifndef VIZZU_DRAWCHART_H
#define VIZZU_DRAWCHART_H

#include "drawingcontext.h"

namespace Vizzu::Draw
{

class DrawChart : public DrawingContext
{
	template <class T, class MemberGetter>
	void drawHeading(const MemberGetter &&getter);

public:
	void draw();
};

}

#endif
