#ifndef LOGO_H
#define LOGO_H

#include <array>
#include <cstdint>

#include "base/gfx/canvas.h"
#include "base/gfx/colortransform.h"

namespace Vizzu::Draw
{

class Logo
{
public:
	explicit Logo(Gfx::ICanvas &canvas) : canvas(canvas) {}
	void draw(Geom::Point pos,
	    double width,
	    const Gfx::ColorTransform &colorTransform);

	static double height(double width);

private:
	struct Point
	{
		uint16_t x;
		uint16_t y;
	};
	using Points =
	    std::array<Point, 6 + 4 + 4 + 4 + (10 + 4 * 2) + 4>;

	static const Points points;
	static const size_t defaultWidth = 2900;
	static const size_t defaultHeight = 766;
	static const size_t defaultRadius;

	Gfx::ICanvas &canvas;
	size_t index{};
	Geom::Point pos;
	double factor{};

	Geom::Point popPoint();
	void popPolygon(size_t count);
	void popPoints(size_t count);
	void popBeziers();
	void popCircle(const Gfx::Color &color);
	void setColor(const Gfx::Color &color);
};
}

#endif
