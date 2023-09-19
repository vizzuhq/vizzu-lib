
#include "pathsampler.h"

namespace Vizzu::Draw
{

PathSampler::PathSampler(const Geom::Point &p0,
    const Geom::Point &p1,
    const PathSampler::Options &options) :
    Gfx::PathSampler(
        options.resolutionMode == ResolutionMode::Low ? 5.0 : 1.0,
        options.resolutionMode == ResolutionMode::Low ? 1.5 : 0.5),
    drawOptions(options),
    p0(p0),
    p1(p1)
{}

}