#ifndef SCALESTATS_H
#define SCALESTATS_H

#include <vector>

#include "base/math/range.h"
#include "chart/options/scales.h"
#include "data/datacube/datacube.h"

#include "diagramitem.h"

namespace Vizzu
{
namespace Diag
{

class ScaleStats
{
public:
	bool discrete;
	Math::Range<double> range;
	double sum;
	std::vector<Data::MultiDim::SubSliceIndex> usedIndices;

	ScaleStats() : discrete(true) {}
	ScaleStats(const Scale &scale,
			   const Data::DataCube &cube);

	void track(double value);
	void trackSingle(double value);
	void track(const DiagramItem::Id &id);
};

class ScalesStats
{
public:
	ScalesStats() = default;
	ScalesStats(const Scales &scales,
				const Data::DataCube &cube);

	std::array<ScaleStats, Scale::Type::id_size> scales;
};

}
}

#endif
