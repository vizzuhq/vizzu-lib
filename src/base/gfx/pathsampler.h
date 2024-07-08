#ifndef GFX_PATHSAMPLER
#define GFX_PATHSAMPLER

#include <cstddef>

#include "base/geom/point.h"

namespace Gfx
{

class PathSampler
{
public:
	struct Options
	{
		double dMax;
		double hMax;
	};
	explicit PathSampler(const Options &options) : options(options) {}

	virtual ~PathSampler() = default;

protected:
	const Options &options;

	virtual void addPoint(const Geom::Point &) = 0;
	virtual Geom::Point getPoint(double f) = 0;

	void calc();

private:
	void path(const Geom::Point &pConv0,
	    const Geom::Point &pConv1,
	    double i0,
	    double i1,
	    size_t recurseCnt = 0);
};

}

#endif
