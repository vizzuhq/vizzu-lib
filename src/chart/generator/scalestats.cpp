#include "scalestats.h"

using namespace Vizzu;
using namespace Vizzu::Diag;

ScaleStats::ScaleStats(const Scale &scale,
					   const Data::DataCube &cube)
{
	sum = 0.0;
	discrete = scale.isPseudoDiscrete();
	if (discrete)
		usedIndices = std::vector<Data::MultiDim::SubSliceIndex>
								 (cube.combinedSizeOf(scale.discretesIds()),
								 Data::MultiDim::SubSliceIndex());
}

void ScaleStats::track(double value)
{
	if (discrete)
		throw std::logic_error("internal error: invalid discrete scale tracking");
	else
		range.include(value);
}

void ScaleStats::trackSingle(double value)
{
	if (discrete)
		throw std::logic_error("internal error: invalid discrete scale tracking");
	else
		sum += value;
}

void ScaleStats::track(const Marker::Id &id)
{
	if (discrete)
		usedIndices[id.itemId] = id.itemSliceIndex;
	else
		throw std::logic_error("internal error: invalid continous scale tracking");
}

ScalesStats::ScalesStats(const Scales &scales,
						 const Data::DataCube &cube)
{
	for (auto scaleId = 0u; scaleId < ScaleId::EnumInfo::count(); scaleId++)
	{
		const auto &scale = scales.at(ScaleId(scaleId));

		this->scales[ScaleId(scaleId)] = ScaleStats(scale, cube);
	}
}
