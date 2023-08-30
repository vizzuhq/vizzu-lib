#ifndef CHART_SPECLAYOUT_H
#define CHART_SPECLAYOUT_H

#include "chart/generator/plot.h"

namespace Vizzu::Gen
{

class SpecLayout
{
public:
	explicit SpecLayout(Plot &plot) : plot(plot) {}
	bool addIfNeeded();

private:
	Plot &plot;
};

}

#endif
