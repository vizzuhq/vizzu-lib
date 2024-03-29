#ifndef PATHSAMPLER_H
#define PATHSAMPLER_H

#include "base/gfx/pathsampler.h"

#include "coordinatesystem.h"
#include "painteroptions.h"

namespace Vizzu::Draw
{

class PathSampler : public Gfx::PathSampler
{
public:
	struct Options
	{
		explicit Options(const CoordinateSystem &coordSys) :
		    coordSys(coordSys)
		{}

		ResolutionMode resolutionMode{ResolutionMode::Low};
		const CoordinateSystem &coordSys;
	};

	PathSampler(const Geom::Point &p0,
	    const Geom::Point &p1,
	    const Options &options);

protected:
	const Options &drawOptions;
	Geom::Point p0;
	Geom::Point p1;
};

}

#endif
