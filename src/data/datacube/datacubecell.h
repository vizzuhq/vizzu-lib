#ifndef DATACUBECELL_H
#define DATACUBECELL_H

#include <vector>

#include "seriesindex.h"
#include "aggregator.h"

namespace Vizzu
{
namespace Data
{

class DataCubeCell
{
public:

	DataCubeCell() {}

	DataCubeCell(const std::vector<SeriesIndex> &indices)
	{
		for (auto &index : indices)
			subCells.push_back(Aggregator(index.getType().aggregatorType()));
	}

	std::vector<Aggregator> subCells;
};

}
}

#endif
