#include "logo.h"

#include <array>

using namespace Vizzu;
using namespace Vizzu::Draw;


const size_t Logo::defaultRadius = 98;

const std::array<Gfx::Color, 4>
Logo::circleColors{{
    Gfx::Color("#dd4d3e"),
    Gfx::Color("#e0cf4b"),
    Gfx::Color("#0085c5"),
    Gfx::Color("#4e9285")
}};

//todo: create Gfx::Path, Gfx::Drawing classes
const Logo::Points Logo::points
{{
	{56, 0}, {370, 760}, {684, 0}, {532, 0}, {370, 425}, {208, 0}, // V
	{784, 0}, {784, 752}, {924, 752}, {924, 0}, // I
	{1188, 752}, {1620, 0}, {1464, 0}, {1034, 752}, // Z
	{1862, 752}, {2294, 0}, {2138, 0}, {1708, 752}, // Z

	// U
	{2530, 0},
	{2390, 0},
	{2390, 480},
	{2390, 650}, {2522, 766}, {2673, 766},
	{2824, 766}, {2956, 650}, {2956, 480},
	{2956, 0},
	{2816, 0},
	{2816, 480},
	{2820, 578}, {2762, 634}, {2673, 634},
	{2584, 634}, {2530, 578}, {2530, 480},

	// circles
	{1146, 99},
	{1820, 99},
	{1522, 652},
	{2196, 652}
}};

void Logo::draw(Geom::Point pos,
    double width,
    bool useDefaultColors,
    const Gfx::Color &color)
{
	this->index = 0;
	this->pos = pos;
	this->factor = width / defaultSize;

	setColor(useDefaultColors ? Gfx::Color::Black() : color);

	popPolygon(6);
	popPolygon(4);
	popPolygon(4);
	popPolygon(4);

	canvas.beginPolygon();
	popPoints(3);
	popBeziers();
	popPoints(3);
	popBeziers();
	canvas.endPolygon();

	for (auto i = 0u; i < 4; i++)
		popCircle(useDefaultColors ? circleColors[i] : color);
}

void Logo::popPolygon(size_t count)
{
	canvas.beginPolygon();
	popPoints(count);
	canvas.endPolygon();
}

void Logo::popPoints(size_t count)
{
	for (auto i = 0u; i < count; i++)
		canvas.addPoint(popPoint());
}

void Logo::popBeziers()
{
	canvas.addBezier(popPoint(), popPoint(), popPoint());
	canvas.addBezier(popPoint(), popPoint(), popPoint());
}

void Logo::popCircle(const Gfx::Color &color)
{
	setColor(color);
	Geom::Circle circle(popPoint(), defaultRadius * factor);
	canvas.circle(circle);
}

Geom::Point Logo::popPoint()
{
	const auto &p = points[index];
	index++;
	return pos + Geom::Point(p.x, p.y) * factor;
}

void Logo::setColor(const Gfx::Color &color)
{
	canvas.setBrushColor(color);
	canvas.setLineColor(color);
}
