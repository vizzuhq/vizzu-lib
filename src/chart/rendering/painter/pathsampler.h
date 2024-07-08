#ifndef PATHSAMPLER_H
#define PATHSAMPLER_H

#include "base/gfx/pathsampler.h"

#include "coordinatesystem.h"

namespace Vizzu::Draw
{

class PathSampler : public Gfx::PathSampler
{
public:
	struct Options : Gfx::PathSampler::Options
	{
		const CoordinateSystem &coordSys;
	};

	PathSampler(const Geom::Point &p0,
	    const Geom::Point &p1,
	    const Options &options);

protected:
	const CoordinateSystem &coordSys;
	Geom::Point p0;
	Geom::Point p1;
};

}

#endif
