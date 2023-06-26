#include "painter.h"

#include "drawline.h"
#include "drawpolygon.h"

using namespace Vizzu;
using namespace Vizzu::Draw;

void Painter::drawLine(const Geom::Line &line)
{
	Draw::drawLine(line, mode, system, getCanvas());
}

void Painter::drawStraightLine(const Geom::Line &line,
    std::array<double, 2> widths,
    double straightFactor,
    const Gfx::Color &endColor,
    const Gfx::Color &lineColor)
{
	Draw::drawLine(line,
	    widths,
	    straightFactor,
	    endColor,
	    lineColor,
	    system,
	    getCanvas());
}

void Painter::drawPolygon(
    const std::array<Geom::Point, 4> &ps,
    bool clip)
{
	Draw::drawPolygon::Options options(system);
	options.circ = polygonOptions.toCircleFactor;
	options.linear = polygonOptions.straightFactor;
	Draw::drawPolygon(ps, options, getCanvas(), clip);
}
