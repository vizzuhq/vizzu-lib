#include "widget.h"

#include "base/util/templates.h"

using namespace GUI;


void Widget::updateSize(Gfx::ICanvas &canvas, Geom::Size &size)
{
	onUpdateSize(canvas, size);
	this->size = size;
}

void Widget::draw(Gfx::ICanvas &canvas)
{
	if (canvas.getClipRect().intersects(Geom::Rect{Geom::Point{}, size})) { onDraw(canvas); }
}