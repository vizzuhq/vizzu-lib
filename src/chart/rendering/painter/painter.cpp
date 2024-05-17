#include "painter.h"

#include "drawline.h"
#include "drawpolygon.h"

namespace Vizzu::Draw
{

void Painter::drawLine(const Geom::Line &line)
{
	Draw::DrawLine(line, mode, system, getCanvas());
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
	Draw::DrawPolygon::Options options(system);
	options.circ = polygonOptions.toCircleFactor;
	options.linear = polygonOptions.straightFactor;
	options.resolutionMode = mode;
	Draw::DrawPolygon(ps, options, getCanvas(), clip);
}

}