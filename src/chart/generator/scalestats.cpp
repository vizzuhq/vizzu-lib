#include "scalestats.h"

using namespace Vizzu;
using namespace Vizzu::Gen;

ScaleStats::ScaleStats(const Scale &scale, const Data::DataCube &cube)
{
	sum = 0.0;
	isDimension = scale.isPseudoDimension();
	if (isDimension)
		usedIndices = std::vector<Data::MultiDim::SubSliceIndex>(
		    cube.combinedSizeOf(scale.dimensionIds()),
		    Data::MultiDim::SubSliceIndex());
}

void ScaleStats::track(double value)
{
	if (isDimension)
		throw std::logic_error(
		    "internal error: invalid isDimension scale tracking");
	else
		range.include(value);
}

void ScaleStats::trackSingle(double value)
{
	if (isDimension)
		throw std::logic_error(
		    "internal error: invalid isDimension scale tracking");
	else
		sum += value;
}

void ScaleStats::track(const Marker::Id &id)
{
	if (isDimension)
		usedIndices[id.itemId] = id.itemSliceIndex;
	else
		throw std::logic_error(
		    "internal error: invalid measure scale tracking");
}

ScalesStats::ScalesStats(const Scales &scales,
    const Data::DataCube &cube)
{
	for (auto scaleId = 0u; scaleId < ScaleId::EnumInfo::count();
	     scaleId++) {
		const auto &scale = scales.at(ScaleId(scaleId));

		this->scales[ScaleId(scaleId)] = ScaleStats(scale, cube);
	}
}
