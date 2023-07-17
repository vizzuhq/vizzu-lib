#ifndef MARKERS_CIRCLEMARKER_H
#define MARKERS_CIRCLEMARKER_H

#include "chart/rendering/painter/coordinatesystem.h"

#include "abstractmarker.h"

namespace Vizzu
{
namespace Draw
{

class CircleMarker : public SingleDrawMarker
{
public:
	CircleMarker(const Gen::Marker &marker,
	    const CoordinateSystem &coordSys,
	    const Gen::Options &options,
	    const Styles::Chart &style);
};

static_assert(sizeof(CircleMarker) == sizeof(AbstractMarker));

}
}

#endif
