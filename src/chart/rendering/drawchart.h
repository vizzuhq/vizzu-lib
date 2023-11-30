
#ifndef VIZZU_DRAWCHART_H
#define VIZZU_DRAWCHART_H

#include "drawingcontext.h"

namespace Vizzu::Draw
{

class DrawChart : public DrawingContext
{
	template <class T>
	void drawHeading(const Gen::Options::Heading &option,
	    const Styles::Label &style,
	    const Geom::Rect &layout,
	    const Util::EventDispatcher::event_ptr &event);

public:
	void draw();
};

}

#endif
