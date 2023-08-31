#ifndef DATACUBECELL_H
#define DATACUBECELL_H

#include <vector>

#include "aggregator.h"
#include "seriesindex.h"

namespace Vizzu::Data
{

class DataCubeCell
{
public:
	DataCubeCell() = default;

	explicit DataCubeCell(const std::vector<SeriesIndex> &indices)
	{
		for (const auto &index : indices)
			subCells.emplace_back(index.getType().aggregatorType());
	}

	std::vector<Aggregator> subCells;
};

}

#endif
