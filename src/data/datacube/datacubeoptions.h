#ifndef DATACUBEOPTIONS_H
#define DATACUBEOPTIONS_H

#include <set>
#include <vector>

#include "seriesindex.h"

namespace Vizzu::Data
{

class DataCubeOptions
{
public:
	using IndexSet = std::set<SeriesIndex>;
	using IndexVector = std::vector<SeriesIndex>;

	DataCubeOptions(const IndexSet &dims, const IndexSet &sers)
	{
		dimensions.insert(dimensions.end(), dims.begin(), dims.end());
		series.insert(series.end(), sers.begin(), sers.end());
	}

	[[nodiscard]] const IndexVector &getDimensions() const
	{
		return dimensions;
	}
	[[nodiscard]] const IndexVector &getSeries() const
	{
		return series;
	}

private:
	IndexVector dimensions;
	IndexVector series;
};

}

#endif
