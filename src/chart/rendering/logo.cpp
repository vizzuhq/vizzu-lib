#include "logo.h"

#include <array>
#include <cstddef>

#include "base/geom/circle.h"
#include "base/geom/point.h"
#include "base/gfx/color.h"
#include "base/gfx/colortransform.h"

namespace Vizzu::Draw
{

const size_t Logo::defaultRadius = 98;

const Logo::Points Logo::points{{{0, 0},
    {314, 760},
    {628, 0},
    {476, 0},
    {314, 425},
    {152, 0}, // V
    {728, 0},
    {728, 752},
    {868, 752},
    {868, 0}, // I
    {1132, 752},
    {1564, 0},
    {1408, 0},
    {978, 752}, // Z
    {1806, 752},
    {2238, 0},
    {2082, 0},
    {1652, 752}, // Z

    // U
    {2474, 0},
    {2334, 0},
    {2334, 480},
    {2334, 650},
    {2466, 766},
    {2617, 766},
    {2768, 766},
    {2900, 650},
    {2900, 480},
    {2900, 0},
    {2760, 0},
    {2760, 480},
    {2764, 578},
    {2706, 634},
    {2617, 634},
    {2528, 634},
    {2474, 578},
    {2474, 480},

    // circles
    {1090, 99},
    {1764, 99},
    {1466, 652},
    {2140, 652}}};

double Logo::height(double width)
{
	return defaultHeight * width / defaultWidth;
}

void Logo::draw(Geom::Point pos,
    double width,
    const Gfx::ColorTransform &colorTransform)
{
	this->index = 0;
	this->pos = pos;
	this->factor = width / defaultWidth;

	canvas.save();

	setColor(colorTransform(Gfx::Color::Black()));
	canvas.setLineWidth(0);

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

	static const std::array<Gfx::Color, 4> circleColors{
	    {Gfx::Color("#dd4d3e"),
	        Gfx::Color("#e0cf4b"),
	        Gfx::Color("#0085c5"),
	        Gfx::Color("#4e9285")}};

	for (auto i = 0U; i < 4; ++i)
		popCircle(colorTransform(circleColors[i]));

	canvas.restore();
}

void Logo::popPolygon(size_t count)
{
	canvas.beginPolygon();
	popPoints(count);
	canvas.endPolygon();
}

void Logo::popPoints(size_t count)
{
	for (auto i = 0U; i < count; ++i) canvas.addPoint(popPoint());
}

void Logo::popBeziers()
{
	canvas.addBezier(popPoint(), popPoint(), popPoint());
	canvas.addBezier(popPoint(), popPoint(), popPoint());
}

void Logo::popCircle(const Gfx::Color &color)
{
	setColor(color);
	const Geom::Circle circle(popPoint(), defaultRadius * factor);
	canvas.circle(circle);
}

Geom::Point Logo::popPoint()
{
	const auto &p = points[index++];
	return pos
	     + Geom::Point{static_cast<double>(p.x),
	           static_cast<double>(p.y)}
	           * factor;
}

void Logo::setColor(const Gfx::Color &color)
{
	canvas.setBrushColor(color);
	canvas.setLineColor(color);
}

}