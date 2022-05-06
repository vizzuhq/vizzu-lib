#ifndef DATACUBEOPTIONS_H
#define DATACUBEOPTIONS_H

#include <vector>
#include <set>
#include "seriesindex.h"

namespace Vizzu
{
namespace Data
{

class DataCubeOptions
{
public:
	typedef std::set<SeriesIndex> IndexSet;
	typedef std::vector<SeriesIndex> IndexVector;

	DataCubeOptions(const IndexSet &dims, const IndexSet &sers)
	{
		dimensions.insert(dimensions.end(), dims.begin(), dims.end());
		series.insert(series.end(), sers.begin(), sers.end());
	}

	const IndexVector &getDimensions() const { return dimensions; }
	const IndexVector &getSeries() const { return series; }

private:
	IndexVector dimensions;
	IndexVector series;
};

}
}

#endif

