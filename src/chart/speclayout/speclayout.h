#ifndef CHART_SPECLAYOUT_H
#define CHART_SPECLAYOUT_H

#include "chart/generator/plot.h"

namespace Vizzu
{
namespace Gen
{

class SpecLayout
{
public:
	SpecLayout(Plot &plot) : plot(plot) {}
	bool addIfNeeded();

private:
	Plot &plot;
};

}
}

#endif
