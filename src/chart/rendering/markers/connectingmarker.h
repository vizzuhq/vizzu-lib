#ifndef MARKERS_CONNECTINGMARKER_H
#define MARKERS_CONNECTINGMARKER_H

#include "abstractmarker.h"

namespace Vizzu::Draw
{

class ConnectingMarker : public AbstractMarker
{
public:
	ConnectingMarker(const Gen::Marker &marker,
	    const CoordinateSystem &coordSys,
	    const Gen::Options &options,
	    const Styles::Chart &style,
	    const Gen::Plot::Markers &markers,
	    size_t lineIndex,
	    Gen::ShapeType type);

	static const Gen::Marker *getPrev(const Gen::Marker &marker,
	    const Gen::Plot::Markers &markers,
	    size_t lineIndex);
};

}

#endif
