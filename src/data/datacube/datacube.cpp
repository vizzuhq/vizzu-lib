#include "datacube.h"

#include "data/table/datatable.h"

namespace Vizzu::Data
{

using MultiDim::DimIndex;
using MultiDim::SubSliceIndex;

DataCube::DataCube(const DataTable &table,
    const DataCubeOptions &options,
    const Filter &filter) :
    table(&table)
{
	if (options.getDimensions().empty()
	    && options.getMeasures().empty())
		return;

	MultiIndex sizes;
	for (auto idx : options.getDimensions()) {
		auto size =
		    idx.getType().isReal()
		        ? table.getInfo(idx.getColIndex().value())
		              .categories()
		              .size()
		    : idx.getType() == SeriesType::Index
		        ? table.getRowCount()
		        : throw std::logic_error("internal error: cannot "
		                                 "tell size of series type");

		sizes.emplace_back(size);

		seriesByDim.push_back(idx);
		dimBySeries.insert({idx, DimIndex(sizes.size() - 1)});
	}

	auto series = options.getMeasures();

	if (series.empty()) series.emplace(SeriesType::Exists);

	data = Data(sizes, DataCubeCell(series));

	for (auto idx : series) {
		seriesBySubIndex.push_back(idx);
		subIndexBySeries.insert(
		    {idx, SubCellIndex(seriesBySubIndex.size() - 1)});
	}

	for (auto rowIdx = 0U; rowIdx < table.getRowCount(); ++rowIdx) {
		const auto &row = table[rowIdx];

		if (!filter.match(RowWrapper(table, row))) continue;

		for (auto cellIt = data.at(getIndex(row,
		                               options.getDimensions(),
		                               rowIdx))
		                       .subCells.begin();
		     const auto &seriesIdx : series)
			cellIt++->add(
			    seriesIdx.getType().isReal()
			        ? double{row[seriesIdx.getColIndex().value()]}
			        : double{});
	}
}

DataCube::MultiIndex DataCube::getIndex(const DataTable::Row &row,
    const std::set<SeriesIndex> &indices,
    size_t rowIndex)
{
	MultiIndex index;
	for (auto idx : indices) {
		auto indexValue =
		    idx.getType().isReal() ? static_cast<std::size_t>(
		        static_cast<double>(row[idx.getColIndex().value()]))
		    : idx.getType() == SeriesType::Index
		        ? rowIndex
		        : throw std::logic_error("internal error: cannot "
		                                 "tell size of series type");

		index.emplace_back(indexValue);
	}
	return index;
}

DimIndex DataCube::getDimBySeries(const SeriesIndex &index) const
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

	for (auto i = 0U; i < multiIndex.size(); ++i)
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
    const MultiIndex &index) const
{
	CellInfo::Categories res;

	for (auto i = 0U; i < index.size(); ++i) {
		auto series = getSeriesByDim(MultiDim::DimIndex{i});
		res.emplace_back(series, index[i]);
	}
	return res;
}

CellInfo::Values DataCube::values(const MultiIndex &index) const
{
	CellInfo::Values res;

	const auto &cell = data.at(index);

	for (auto i = 0U; i < cell.subCells.size(); ++i) {
		auto series = getSeriesBySubIndex(SubCellIndex{i});

		if (series.getType() == SeriesType::Exists) continue;

		res.emplace_back(series, cell.subCells[i]);
	}
	return res;
}

CellInfo DataCube::cellInfo(const MultiIndex &index) const
{
	if (!table) return {};

	return {categories(index), values(index)};
}

}