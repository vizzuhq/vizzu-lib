#include "datacube.h"

#include "data/table/datatable.h"

Vizzu::Data::DataCube::DataCube(const DataTable &table,
    const DataCubeOptions &options,
    const Filter &filter) :
    table(&table)
{
	if (options.getDimensions().empty()
	    && options.getSeries().empty())
		return;

	MultiDim::MultiIndex sizes;
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
		dimBySeries.insert(
		    {idx, MultiDim::DimIndex(sizes.size() - 1)});
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

Vizzu::Data::MultiDim::MultiIndex Vizzu::Data::DataCube::getIndex(
    const TableRow<double> &row,
    const std::vector<SeriesIndex> &indices,
    size_t rowIndex)
{
	MultiDim::MultiIndex index;
	for (auto idx : indices) {
		auto indexValue =
		    idx.getType().isReal()
		        ? static_cast<size_t>(row[idx.getColIndex().value()])
		    : idx.getType() == SeriesType::Index
		        ? rowIndex
		        : throw std::logic_error("internal error: cannot "
		                                 "tell size of series type");

		index.emplace_back(indexValue);
	}
	return index;
}

Vizzu::Data::MultiDim::DimIndex Vizzu::Data::DataCube::getDimBySeries(
    const SeriesIndex &index) const
{
	if (auto it = dimBySeries.find(index);
	    it != std::end(dimBySeries)) [[likely]]
		return it->second;

	throw std::logic_error(
	    "internal error, table column is not in data cube");
}

Vizzu::Data::SeriesIndex Vizzu::Data::DataCube::getSeriesByDim(
    MultiDim::DimIndex index) const
{
	if (seriesByDim.size() > index) [[likely]]
		return seriesByDim[index];

	throw std::logic_error(
	    "internal error, dimension index out of range");
}

Vizzu::Data::SeriesIndex Vizzu::Data::DataCube::getSeriesBySubIndex(
    SubCellIndex index) const
{
	if (seriesBySubIndex.size() > index) [[likely]]
		return seriesBySubIndex[index];

	throw std::logic_error(
	    "internal error, sub-cell index out of range");
}

Vizzu::Data::MultiDim::SubSliceIndex
Vizzu::Data::DataCube::subSliceIndex(const SeriesList &colIndices,
    MultiDim::MultiIndex multiIndex) const
{
	MultiDim::SubSliceIndex subSliceIndex;
	for (auto colIndex : colIndices) {
		auto dimIndex = getDimBySeries(colIndex);
		subSliceIndex.push_back({dimIndex, multiIndex[dimIndex]});
	}
	return subSliceIndex;
}

size_t Vizzu::Data::DataCube::subCellSize() const
{
	return seriesBySubIndex.size();
}

bool Vizzu::Data::DataCube::empty() const { return data.empty(); }

Vizzu::Data::MultiDim::SubSliceIndex
Vizzu::Data::DataCube::inverseSubSliceIndex(
    const SeriesList &colIndices,
    MultiDim::MultiIndex multiIndex) const
{
	MultiDim::SubSliceIndex subSliceIndex;
	subSliceIndex.reserve(multiIndex.size());

	std::set<MultiDim::DimIndex> dimIndices;
	for (auto colIndex : colIndices)
		dimIndices.insert(getDimBySeries(colIndex));

	for (auto i = 0U; i < multiIndex.size(); i++)
		if (dimIndices.find(MultiDim::DimIndex(i))
		    == dimIndices.end())
			subSliceIndex.push_back(
			    {MultiDim::DimIndex(i), multiIndex[i]});

	return subSliceIndex;
}

size_t Vizzu::Data::DataCube::combinedIndexOf(
    const SeriesList &colIndices,
    MultiDim::MultiIndex multiIndex) const
{
	return data.unfoldSubSliceIndex(
	    subSliceIndex(colIndices, std::move(multiIndex)));
}

size_t Vizzu::Data::DataCube::combinedSizeOf(
    const SeriesList &colIndices) const
{
	return combinedIndexOf(colIndices, data.maxIndex()) + 1;
}

Vizzu::Data::Aggregator Vizzu::Data::DataCube::aggregateAt(
    const MultiDim::MultiIndex &multiIndex,
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

double Vizzu::Data::DataCube::sumTillAt(const SeriesList &colIndices,
    const SeriesList &sumCols,
    const MultiDim::MultiIndex &multiIndex,
    SeriesIndex seriesId) const
{
	double sum = 0;

	data.visitSubSlicesTill(subSliceIndex(colIndices, multiIndex),
	    [this, &multiIndex, &sum, &sumCols, &seriesId](
	        const MultiDim::SubSliceIndex &subSliceIndex)
	    {
		    auto index = subSliceIndex.getProjectionOf(multiIndex);
		    sum += static_cast<double>(
		        aggregateAt(index, sumCols, seriesId));
	    });

	return sum;
}

Vizzu::Data::Aggregator Vizzu::Data::DataCube::valueAt(
    const MultiDim::MultiIndex &multiIndex,
    const SeriesIndex &seriesId) const
{
	auto subCellIndex = subIndexBySeries.at(seriesId);
	return data.at(multiIndex).subCells[subCellIndex];
}

size_t Vizzu::Data::DataCube::subSliceID(const SeriesList &colIndices,
    const MultiDim::MultiIndex &multiIndex) const
{
	return data.unfoldSubSliceIndex(
	    inverseSubSliceIndex(colIndices, multiIndex));
}

size_t Vizzu::Data::DataCube::flatSubSliceIndex(
    const SeriesList &colIndices,
    const MultiDim::MultiIndex &multiIndex) const
{
	return data.unfoldSubSliceIndex(
	    subSliceIndex(colIndices, multiIndex));
}

Vizzu::Data::CellInfo::Categories Vizzu::Data::DataCube::categories(
    const MultiDim::MultiIndex &index) const
{
	CellInfo::Categories res;

	for (auto i = 0U; i < index.size(); i++) {
		auto series = getSeriesByDim(MultiDim::DimIndex{i});
		res.emplace_back(series, index[i]);
	}
	return res;
}

Vizzu::Data::CellInfo::Values Vizzu::Data::DataCube::values(
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

Vizzu::Data::CellInfo Vizzu::Data::DataCube::cellInfo(
    const MultiDim::MultiIndex &index) const
{
	if (!table) return {};

	return {categories(index), values(index)};
}

Vizzu::Data::MultiDim::SubSliceIndex
Vizzu::Data::DataCube::subSliceIndex(
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