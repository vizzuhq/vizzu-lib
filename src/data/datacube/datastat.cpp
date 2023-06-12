#include "datastat.h"

using namespace Vizzu;
using namespace Vizzu::Data;

DataStat::DataStat(const DataTable &table,
    const DataCubeOptions &options,
    const Filter &filter)
{
	const auto &indices = options.getDimensions();
	for (const auto &idx : indices) {
		if (idx.getType().isReal()) {
			auto valueCnt =
			    table.getInfo(idx.getColIndex()).discreteValueCnt();
			usedColumnIDs.insert(
			    {(size_t)idx.getColIndex(), usedValues.size()});
			usedValues.emplace_back();
			usedValues.back().resize(valueCnt);
		}
	}

	for (auto rowIdx = 0u; rowIdx < table.getRowCount(); rowIdx++) {
		const auto &row = table[rowIdx];

		if (filter.match(RowWrapper(table, row)))
			trackIndex(row, options.getDimensions());
	}

	countValues();
}

size_t DataStat::usedValueCntOf(const SeriesIndex &index) const
{
	auto it = usedColumnIDs.find((size_t)index.getColIndex());
	if (it != usedColumnIDs.end()) return usedValueCnt[it->second];
	return 0;
}

void DataStat::trackIndex(const TableRow<double> &row,
    const std::vector<SeriesIndex> &indices)
{
	for (auto i = 0u; i < indices.size(); i++) {
		const auto &idx = indices[i];
		if (idx.getType().isReal())
			usedValues[i][row[idx.getColIndex()]] = true;
	}
}

void DataStat::countValues()
{
	for (const auto &values : usedValues) {
		auto cnt = 0;
		for (auto used : values)
			if (used) cnt++;
		usedValueCnt.push_back(cnt);
	}
}
