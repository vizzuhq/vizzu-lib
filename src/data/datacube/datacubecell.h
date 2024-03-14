#ifndef DATACUBECELL_H
#define DATACUBECELL_H

#include <set>
#include <vector>

#include "aggregator.h"
#include "seriesindex.h"

namespace Vizzu::Data
{

class DataCubeCell
{
public:
	DataCubeCell() = default;

	explicit DataCubeCell(const std::set<SeriesIndex> &indices)
	{
		for (const auto &index : indices)
			subCells.emplace_back(index.getType().aggregatorType());
	}

	std::vector<Aggregator> subCells;
};

}

#endif
