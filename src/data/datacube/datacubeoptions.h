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

	DataCubeOptions(const IndexSet &dims, const IndexSet &msrs)
	{
		dimensions.insert(dimensions.end(), dims.begin(), dims.end());
		measures.insert(measures.end(), msrs.begin(), msrs.end());
	}

	[[nodiscard]] const IndexVector &getDimensions() const
	{
		return dimensions;
	}

	[[nodiscard]] const IndexVector &getMeasures() const
	{
		return measures;
	}

private:
	IndexVector dimensions;
	IndexVector measures;
};

}

#endif
