
#include "pathsampler.h"

#include "base/geom/point.h"
#include "base/gfx/pathsampler.h"

namespace Vizzu::Draw
{

PathSampler::PathSampler(const Geom::Point &p0,
    const Geom::Point &p1,
    const Options &options) :
    Gfx::PathSampler(options),
    coordSys(options.coordSys),
    p0(p0),
    p1(p1)
{}

}