#include "circlemarker.h"

namespace Vizzu::Draw
{

CircleMarker::CircleMarker(const Gen::Marker &marker,
    const CoordinateSystem &coordSys,
    const Gen::Options &options,
    const Styles::Chart &style) :
    SingleDrawMarker(marker, options, Gen::ShapeType::circle)
{
	morphToCircle = true;
	border = false;
	auto spacing = marker.spacing * marker.size / 2;
	auto pos = marker.position - spacing;
	auto r = *style.plot.marker.circleMaxRadius
	       * sqrt(std::max(0.0, marker.sizeFactor));
	r = std::max(r, *style.plot.marker.circleMinRadius);
	points[0] = pos + Geom::Point{-r, -r};
	points[1] = pos + Geom::Point{+r, -r};
	points[2] = pos + Geom::Point{+r, +r};
	points[3] = pos + Geom::Point{-r, +r};
	center = pos;
	lineWidth[0] = lineWidth[1] = 0;
	dataRect.pos = pos;
	dataRect.size = Geom::Size();
	radius = fabs(coordSys.verConvert(r));
}

}