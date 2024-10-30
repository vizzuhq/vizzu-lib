#ifndef MARKERS_RECTANGLEMARKER_H
#define MARKERS_RECTANGLEMARKER_H

#include "abstractmarker.h"

namespace Vizzu::Draw
{

class RectangleMarker : public SingleDrawMarker
{
public:
	RectangleMarker(const Gen::Marker &marker,
	    const CoordinateSystem &coordSys,
	    const Gen::Options &options,
	    const Styles::Chart &style);
};

static_assert(sizeof(RectangleMarker) == sizeof(AbstractMarker));

}

#endif
