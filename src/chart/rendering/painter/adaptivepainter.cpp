#include "adaptivepainter.h"

#include "drawline.h"
#include "drawpolygon.h"

using namespace Vizzu;
using namespace Vizzu::Draw;


void AdaptivePainter::copyOptionsTo(IPainter &painter) const
{
	painter.setCoordSys(system);
	painter.setResMode(mode);
	painter.setPolygonMinDotSize(polygonOptions.minDotSize);
	painter.setPolygonToCircleFactor(polygonOptions.toCircleFactor);
	painter.setPolygonStraightFactor(polygonOptions.straightFactor);
}

void AdaptivePainter::drawLine(const Geom::Line &line)
{
	Draw::drawLine(line, mode, system, getCanvas());
}

void AdaptivePainter::drawStraightLine(const Geom::Line &line,
									   std::array<double, 2> widths,
									   const Gfx::Color &endColor,
									   const Gfx::Color &lineColor)
{
	Draw::drawLine(line, widths, endColor, lineColor, system, getCanvas());
}

void AdaptivePainter::drawPolygon(const std::array<Geom::Point, 4> &ps)
{
	Draw::drawPolygon::Options options(system);
	options.circ = polygonOptions.toCircleFactor;
	options.linear = polygonOptions.straightFactor;
	options.minDotSize = polygonOptions.minDotSize;
	Draw::drawPolygon(ps, options, getCanvas());
}
