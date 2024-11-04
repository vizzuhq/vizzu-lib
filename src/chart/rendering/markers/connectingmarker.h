#ifndef MARKERS_CONNECTINGMARKER_H
#define MARKERS_CONNECTINGMARKER_H

#include "abstractmarker.h"

namespace Vizzu::Draw
{

class ConnectingMarker : public AbstractMarker
{
public:
	ConnectingMarker(const DrawingContext &ctx,
	    const Gen::Marker &marker,
	    ::Anim::InterpolateIndex lineIndex,
	    Gen::ShapeType type);

	static const Gen::Marker *getPrev(const Gen::Marker &marker,
	    ::Anim::InterpolateIndex lineIndex);
};

}

#endif
