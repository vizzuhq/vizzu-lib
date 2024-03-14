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

	DataCubeOptions(IndexSet &&dims, IndexSet &&msrs) :
	    dimensions(std::move(dims)),
	    measures(std::move(msrs))
	{}

	[[nodiscard]] const IndexSet &getDimensions() const
	{
		return dimensions;
	}

	[[nodiscard]] const IndexSet &getMeasures() const
	{
		return measures;
	}

private:
	IndexSet dimensions;
	IndexSet measures;
};

}

#endif
