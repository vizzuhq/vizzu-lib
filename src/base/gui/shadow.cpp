#include "shadow.h"

using namespace GUI;

Shadow::Shadow(Widget *parent, Side side)
	: parent(parent),
	  side(side)
{}

void Shadow::draw(Gfx::ICanvas &canvas)
{
	Gfx::ColorGradient gradient;
	gradient.stops.push_back({ 0.0, Gfx::Color(0, 0, 0, 0.1) });
	gradient.stops.push_back({ 0.4, Gfx::Color(0, 0, 0, 0.05) });
	gradient.stops.push_back({ 1.0, Gfx::Color(0, 0, 0, 0.0) });

	Geom::Line line;
	Geom::Rect rect;
	if (side == Side::Right)
	{
		rect = Geom::Rect(parent->getBoundary().bottomRight(),
						  Geom::Size(7, parent->getBoundary().size.y));
		line = Geom::Line(rect.topLeft(), rect.topRight());
	}
	else
	{
		rect = Geom::Rect(parent->getBoundary().bottomLeft() - Geom::Point::X(7),
						  Geom::Size(7, parent->getBoundary().size.y));
		line = Geom::Line(rect.topRight(), rect.topLeft());
	}
	canvas.setBrushGradient(line, gradient);
	canvas.setLineColor(Gfx::Color::Transparent());
	canvas.rectangle(rect);
}
