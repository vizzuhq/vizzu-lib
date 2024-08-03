#include "painter.h"

#include <array>

#include "base/geom/line.h"
#include "base/geom/point.h"

#include "drawline.h"
#include "drawpolygon.h"

namespace Vizzu::Draw
{

void Painter::drawLine(const Geom::Line &line)
{
	DrawLine(line, {pathSamplerOptions, system}, getCanvas());
}

void Painter::drawStraightLine(const Geom::Line &line,
    std::array<double, 2> widths,
    double straightFactor)
{
	Draw::DrawLine(line, widths, straightFactor, system, getCanvas());
}

void Painter::drawPolygon(const std::array<Geom::Point, 4> &ps,
    bool clip)
{
	Draw::DrawPolygon(ps,
	    {{pathSamplerOptions, system}, polygonOptions},
	    getCanvas(),
	    clip);
}

}