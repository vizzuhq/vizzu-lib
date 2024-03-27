#include "datastat.h"

#include <base/conv/tostring.h>

namespace Vizzu::Data
{

DataStat::DataStat(const DataTable &table,
    const DataCubeOptions &options,
    const Filter &filter)
{
	const auto &indices = options.getDimensions();
	for (const auto &idx : indices) {
		if (idx.getType().isReal()) {
			usedColumnIDs.try_emplace(idx.getColIndex().value(),
			    usedValues.size());
			usedValues.emplace_back();
		}
	}

	for (auto rowIdx = 0U; rowIdx < table.getRowCount(); ++rowIdx)
		if (const auto &row = table[rowIdx];
		    filter.match(RowWrapper(table, row)))
			trackIndex(row, indices);
}

size_t DataStat::usedValueCntOf(const SeriesIndex &index) const
{
	auto it = usedColumnIDs.find(index.getColIndex().value());
	if (it != usedColumnIDs.end())
		return usedValues[it->second].size();
	return 0;
}

void DataStat::trackIndex(const DataTable::Row &row,
    const std::set<SeriesIndex> &indices)
{
	for (auto it = usedValues.begin(); const auto &idx : indices)
		if (idx.getType().isReal())
			it++->emplace(
			    Conv::toString(row[idx.getColIndex().value()]));
}

}