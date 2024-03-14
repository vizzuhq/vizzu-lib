#ifndef DATASTAT_H
#define DATASTAT_H

#include <unordered_map>

#include "datacubeoptions.h"
#include "datafilter.h"

namespace Vizzu::Data
{

class DataStat
{
public:
	DataStat(const DataTable &table,
	    const DataCubeOptions &options,
	    const Filter &filter = Filter());

	size_t usedValueCntOf(const SeriesIndex &index) const;

private:
	void trackIndex(const DataTable::Row &row,
	    const std::set<SeriesIndex> &indices);

	void countValues();

	std::unordered_map<size_t, size_t> usedColumnIDs;
	std::vector<std::vector<bool>> usedValues;
	std::vector<size_t> usedValueCnt;
};

}

#endif
