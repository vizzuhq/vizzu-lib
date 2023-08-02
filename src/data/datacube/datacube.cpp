#include "datacube.h"

#include "data/table/datatable.h"

using namespace Vizzu;
using namespace Vizzu::Data;
using namespace Vizzu::Data::MultiDim;

DataCube::DataCube(const DataTable &table,
    const DataCubeOptions &options,
    const Filter &filter) :
    table(&table)
{
	if (options.getDimensions().empty()
	    && options.getSeries().empty())
		return;

	MultiIndex sizes;
	for (auto idx : options.getDimensions()) {
		auto size =
		    idx.getType().isReal()
		        ? table.getInfo(idx.getColIndex().value())
		              .dimensionValueCnt()
		    : idx.getType() == SeriesType::Index
		        ? table.getRowCount()
		        : throw std::logic_error("internal error: cannot "
		                                 "tell size of series type");

		sizes.emplace_back(size);

		seriesByDim.push_back(idx);
		dimBySeries.insert({idx, DimIndex(sizes.size() - 1)});
	}

	auto series = options.getSeries();

	if (series.empty()) series.emplace_back(SeriesType::Exists);

	data = Data(sizes, DataCubeCell(series));

	for (auto idx : series) {
		seriesBySubIndex.push_back(idx);
		subIndexBySeries.insert(
		    {idx, SubCellIndex(seriesBySubIndex.size() - 1)});
	}

	for (auto rowIdx = 0U; rowIdx < table.getRowCount(); rowIdx++) {
		const auto &row = table[rowIdx];

		auto index = getIndex(row, options.getDimensions(), rowIdx);

		for (auto idx = 0U; idx < series.size(); idx++) {
			auto value = series[idx].getType().isReal()
			               ? row[series[idx].getColIndex().value()]
			               : 0.0;

			if (filter.match(RowWrapper(table, row)))
				data.at(index).subCells[idx].add(value);
		}
	}
}

MultiIndex DataCube::getIndex(const TableRow<double> &row,
    const std::vector<SeriesIndex> &indices,
    size_t rowIndex)
{
	MultiIndex index;
	for (auto idx : indices) {
		auto indexValue =
		    idx.getType().isReal() ? row[idx.getColIndex().value()]
		    : idx.getType() == SeriesType::Index
		        ? rowIndex
		        : throw std::logic_error("internal error: cannot "
		                                 "tell size of series type");

		index.emplace_back(static_cast<size_t>(indexValue));
	}
	return index;
}

DimIndex DataCube::getDimBySeries(SeriesIndex index) const
{
	if (auto it = dimBySeries.find(index);
	    it != std::end(dimBySeries)) [[likely]]
		return it->second;

	throw std::logic_error(
	    "internal error, table column is not in data cube");
}

SeriesIndex DataCube::getSeriesByDim(DimIndex index) const
{
	if (seriesByDim.size() > index) [[likely]]
		return seriesByDim[index];

	throw std::logic_error(
	    "internal error, dimension index out of range");
}

SeriesIndex DataCube::getSeriesBySubIndex(SubCellIndex index) const
{
	if (seriesBySubIndex.size() > index) [[likely]]
		return seriesBySubIndex[index];

	throw std::logic_error(
	    "internal error, sub-cell index out of range");
}

SubSliceIndex DataCube::subSliceIndex(const SeriesList &colIndices,
    MultiIndex multiIndex) const
{
	SubSliceIndex subSliceIndex;
	for (auto colIndex : colIndices) {
		auto dimIndex = getDimBySeries(colIndex);
		subSliceIndex.push_back({dimIndex, multiIndex[dimIndex]});
	}
	return subSliceIndex;
}

size_t DataCube::subCellSize() const
{
	return seriesBySubIndex.size();
}

bool DataCube::empty() const { return data.empty(); }

SubSliceIndex DataCube::inverseSubSliceIndex(
    const SeriesList &colIndices,
    MultiIndex multiIndex) const
{
	SubSliceIndex subSliceIndex;
	subSliceIndex.reserve(multiIndex.size());

	std::set<DimIndex> dimIndices;
	for (auto colIndex : colIndices)
		dimIndices.insert(getDimBySeries(colIndex));

	for (auto i = 0U; i < multiIndex.size(); i++)
		if (dimIndices.find(DimIndex(i)) == dimIndices.end())
			subSliceIndex.push_back({DimIndex(i), multiIndex[i]});

	return subSliceIndex;
}

size_t DataCube::combinedIndexOf(const SeriesList &colIndices,
    MultiIndex multiIndex) const
{
	return data.unfoldSubSliceIndex(
	    subSliceIndex(colIndices, std::move(multiIndex)));
}

size_t DataCube::combinedSizeOf(const SeriesList &colIndices) const
{
	return combinedIndexOf(colIndices, data.maxIndex()) + 1;
}

Aggregator DataCube::aggregateAt(const MultiIndex &multiIndex,
    const SeriesList &sumCols,
    SeriesIndex seriesId) const
{
	Aggregator aggregate(seriesId.getType().aggregatorType());
	auto subCellIndex = subIndexBySeries.at(seriesId);

	data.visitSubSlice(inverseSubSliceIndex(sumCols, multiIndex),
	    [&](const DataCubeCell &cell)
	    {
		    aggregate.add(cell.subCells[subCellIndex]);
	    });

	return aggregate;
}

double DataCube::sumTillAt(const SeriesList &colIndices,
    const SeriesList &sumCols,
    const MultiIndex &multiIndex,
    SeriesIndex seriesId) const
{
	double sum = 0;

	data.visitSubSlicesTill(subSliceIndex(colIndices, multiIndex),
	    [&](const SubSliceIndex &subSliceIndex)
	    {
		    auto index = subSliceIndex.getProjectionOf(multiIndex);
		    sum += static_cast<double>(
		        aggregateAt(index, sumCols, seriesId));
	    });

	return sum;
}

Aggregator DataCube::valueAt(const MultiIndex &multiIndex,
    const SeriesIndex &seriesId) const
{
	auto subCellIndex = subIndexBySeries.at(seriesId);
	return data.at(multiIndex).subCells[subCellIndex];
}

size_t DataCube::subSliceID(const SeriesList &colIndices,
    const MultiIndex &multiIndex) const
{
	return data.unfoldSubSliceIndex(
	    inverseSubSliceIndex(colIndices, multiIndex));
}

size_t DataCube::flatSubSliceIndex(const SeriesList &colIndices,
    const MultiIndex &multiIndex) const
{
	return data.unfoldSubSliceIndex(
	    subSliceIndex(colIndices, multiIndex));
}

CellInfo::Categories DataCube::categories(
    const MultiDim::MultiIndex &index) const
{
	CellInfo::Categories res;

	for (auto i = 0U; i < index.size(); i++) {
		auto series = getSeriesByDim(MultiDim::DimIndex{i});
		res.emplace_back(series, index[i]);
	}
	return res;
}

CellInfo::Values DataCube::values(
    const MultiDim::MultiIndex &index) const
{
	CellInfo::Values res;

	const auto &cell = data.at(index);

	for (auto i = 0U; i < cell.subCells.size(); i++) {
		auto series = getSeriesBySubIndex(SubCellIndex{i});

		if (series.getType() == SeriesType::Exists) continue;

		auto value = static_cast<double>(cell.subCells[i]);

		res.emplace_back(series, value);
	}
	return res;
}

CellInfo DataCube::cellInfo(const MultiDim::MultiIndex &index) const
{
	if (!table) return {};

	return {categories(index), values(index)};
}

MultiDim::SubSliceIndex DataCube::subSliceIndex(
    const MarkerIdStrings &stringMarkerId) const
{
	MultiDim::SubSliceIndex index;
	for (const auto &pair : stringMarkerId) {
		auto colIdx = table->getColumn(pair.first);
		auto seriesIdx = table->getIndex(colIdx);
		const auto &values =
		    table->getInfo(colIdx).dimensionValueIndexes();
		auto valIdx = values.at(pair.second);
		auto dimIdx = getDimBySeries(SeriesIndex(seriesIdx));
		index.push_back(
		    MultiDim::SliceIndex{dimIdx, MultiDim::Index{valIdx}});
	}
	return index;
}