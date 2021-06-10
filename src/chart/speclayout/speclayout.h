#ifndef CHART_SPECLAYOUT_H
#define CHART_SPECLAYOUT_H

#include "chart/generator/diagram.h"

namespace Vizzu
{
namespace Diag
{

class SpecLayout
{
public:
	SpecLayout(Diagram &diagram) : diagram(diagram) {}
	bool addIfNeeded();
private:
	Diagram &diagram;
};

}
}

#endif
