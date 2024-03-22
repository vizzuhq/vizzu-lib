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

	std::unordered_map<SeriesIndex::OptColIndex::value_type, size_t>
	    usedColumnIDs;
	std::vector<std::unordered_set<std::string>> usedValues;
};

}

#endif
