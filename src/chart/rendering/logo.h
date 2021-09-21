#ifndef LOGO_H
#define LOGO_H

#include <array>
#include <cstdint>

#include "base/gfx/canvas.h"
#include "base/gfx/colortransform.h"

namespace Vizzu
{
namespace Draw
{

class Logo
{
public:
	Logo(Gfx::ICanvas &canvas) : canvas(canvas) {}
	void draw(Geom::Point pos, double width,
			  const Gfx::ColorTransform &colorTransform);
	
private:
	struct Point { uint16_t x; uint16_t y; };
	typedef std::array<Point, 6 + 4 + 4 + 4 + (10 + 4*2) + 4> Points;

	static const Points points;
	static const size_t defaultSize = 3000;
	static const size_t defaultRadius;
	static const std::array<Gfx::Color, 4> circleColors;

	Gfx::ICanvas &canvas;
	size_t index;
	Geom::Point pos;
	double factor;

	Geom::Point popPoint();
	void popPolygon(size_t count);
	void popPoints(size_t count);
	void popBeziers();
	void popCircle(const Gfx::Color &color);
	void setColor(const Gfx::Color &color);
};
}
}

#endif
