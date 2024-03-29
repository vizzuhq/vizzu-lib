#include "datastat.h"

namespace Vizzu::Data
{

DataStat::DataStat(const DataTable &table,
    const DataCubeOptions &options,
    const Filter &filter)
{
	const auto &indices = options.getDimensions();
	for (const auto &idx : indices) {
		if (idx.getType().isReal()) {
			auto valueCnt = table.getInfo(idx.getColIndex().value())
			                    .categories()
			                    .size();
			usedColumnIDs.insert(
			    {static_cast<size_t>(idx.getColIndex().value()),
			        usedValues.size()});
			usedValues.emplace_back().resize(valueCnt);
		}
	}

	for (auto rowIdx = 0U; rowIdx < table.getRowCount(); ++rowIdx) {
		const auto &row = table[rowIdx];

		if (filter.match(RowWrapper(table, row)))
			trackIndex(row, indices);
	}

	countValues();
}

size_t DataStat::usedValueCntOf(const SeriesIndex &index) const
{
	auto it = usedColumnIDs.find(
	    static_cast<size_t>(index.getColIndex().value()));
	if (it != usedColumnIDs.end()) return usedValueCnt[it->second];
	return 0;
}

void DataStat::trackIndex(const DataTable::Row &row,
    const std::set<SeriesIndex> &indices)
{
	for (auto it = usedValues.begin(); const auto &idx : indices)
		(*it++)[static_cast<size_t>(row[idx.getColIndex().value()])] =
		    idx.getType().isReal();
}

void DataStat::countValues()
{
	for (const auto &values : usedValues) {
		auto cnt = 0;
		for (auto used : values)
			if (used) ++cnt;
		usedValueCnt.push_back(cnt);
	}
}

}