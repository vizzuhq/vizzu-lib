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

	template <class SI>
	explicit DataCubeCell(const std::set<SI> &indices)
	{
		for (const auto &index : indices)
			subCells.emplace_back(index.getType().aggregatorType());
	}

	[[nodiscard]] bool isEmpty() const
	{
		return subCells[0].isEmpty();
	}

	std::vector<Aggregator> subCells;
};

}

#endif
