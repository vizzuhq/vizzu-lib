#ifndef CHART_SPECLAYOUT_H
#define CHART_SPECLAYOUT_H

#include "chart/generator/plotptr.h"

namespace Vizzu::Gen
{

class SpecLayout
{
public:
	explicit SpecLayout(Plot &plot) : plot(plot) {}
	bool addIfNeeded(Buckets &hierarchy) const;

private:
	Plot &plot;
};

}

#endif
