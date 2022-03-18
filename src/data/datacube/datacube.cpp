#include "datacube.h"

#include "data/table/datatable.h"

using namespace Vizzu;
using namespace Vizzu::Data;
using namespace Vizzu::Data::MultiDim;

DataCube::DataCube(const DataTable &table,
				   const DataCubeOptions &options,
				   const Filter &filter)
	: table(&table)
{
	if(options.getDimensions().empty() 
		&& options.getSeries().empty())
		return;

	MultiIndex sizes;
	for (auto idx : options.getDimensions())
	{
		auto size =
			idx.getType().isReal()					? table.getInfo(idx.getColIndex()).discreteValueCnt() :
			idx.getType() == SeriesType::Index		? table.getRowCount() :
			throw std::logic_error("internal error: cannot tell size of series type");

		sizes.push_back(Index(size));

		seriesByDim.push_back(idx);
		dimBySeries.insert({ idx, DimIndex(sizes.size()-1) });
	}

	auto series = options.getSeries();

	data = Data(sizes, DataCubeCell(series));

	for (auto idx: series)
	{
		seriesBySubIndex.push_back(idx);
		subIndexBySeries.insert({ idx, SubCellIndex(seriesBySubIndex.size()-1) });
	}

	for (auto rowIdx = 0u; rowIdx < table.getRowCount(); rowIdx++)
	{
		const auto &row = table[rowIdx];

		auto index = getIndex(row, options.getDimensions(), rowIdx);

		for (auto idx = 0u; idx < series.size(); idx++)
		{
			auto value = series[idx].getType().isReal()
					? row[series[idx].getColIndex()] : 0.0;

			if (filter.match(RowWrapper(table, row)))
				data.at(index).subCells[idx].add(value, (int)row[ColumnIndex(0u)]);
		}
	}
}

MultiIndex DataCube::getIndex(const TableRow<double> &row,
					const std::vector<SeriesIndex> &indices,
					size_t rowIndex)
{
	MultiIndex index;
	for (auto idx : indices)
	{
		auto indexValue =
			idx.getType().isReal() ? row[idx.getColIndex()] :
			idx.getType() == SeriesType::Index ? rowIndex :
			throw std::logic_error("internal error: cannot tell size of series type");

		index.push_back(MultiDim::Index((size_t)indexValue));
	}
	return index;
}

DimIndex DataCube::getDimBySeries(SeriesIndex index) const
{
	try
	{
		return dimBySeries.at(index);
	}
	catch(...)
	{
		throw std::logic_error
			("internal error, table column is not in data cube");
	}
}

SeriesIndex DataCube::getSeriesByDim(DimIndex index) const
{
	try
	{
		return seriesByDim.at(index);
	}
	catch(...)
	{
		throw std::logic_error
			("internal error, dimension index out of range");
	}
}

SeriesIndex DataCube::getSeriesBySubIndex(SubCellIndex index) const
{
	try
	{
		return seriesBySubIndex.at(index);
	}
	catch(...)
	{
		throw std::logic_error
			("internal error, sub-cell index out of range");
	}
}

SubSliceIndex
DataCube::subSliceIndex(const SeriesList &colIndices,
						MultiIndex multiIndex) const
{
	SubSliceIndex subSliceIndex;
	for (auto colIndex: colIndices) {
		auto dimIndex = getDimBySeries(colIndex);
		subSliceIndex.push_back({ dimIndex, multiIndex[dimIndex] });
	}
	return subSliceIndex;
}

size_t DataCube::subCellSize() const
{
	return seriesBySubIndex.size();
}

bool DataCube::empty() const
{
	return data.empty();
}

SubSliceIndex DataCube::inverseSubSliceIndex(const SeriesList &colIndices,
											 MultiIndex multiIndex) const
{
	SubSliceIndex subSliceIndex;
	subSliceIndex.reserve(multiIndex.size());

	std::set<DimIndex> dimIndices;
	for (auto colIndex: colIndices)
		dimIndices.insert(getDimBySeries(colIndex));

	for (auto i = 0u; i < multiIndex.size(); i++)
		if (dimIndices.find(DimIndex(i)) == dimIndices.end())
			subSliceIndex.push_back({ DimIndex(i), multiIndex[i] });

	return subSliceIndex;
}

size_t DataCube::combinedIndexOf(const SeriesList &colIndices,
								 MultiIndex multiIndex) const
{
	return data.unfoldSubSliceIndex(subSliceIndex(colIndices, std::move(multiIndex)));
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
	[&](const DataCubeCell &cell) {
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
	[&](const SubSliceIndex &subSliceIndex) {
		auto index = subSliceIndex.getProjectionOf(multiIndex);
		sum += (double)aggregateAt(index, sumCols, seriesId);
	});

	return sum;
}

Aggregator DataCube::valueAt(const MultiIndex &multiIndex, const SeriesIndex &seriesId) const
{
	auto subCellIndex = subIndexBySeries.at(seriesId);
	return data.at(multiIndex).subCells[subCellIndex];
}

size_t DataCube::subSliceID(const SeriesList &colIndices, const MultiIndex &multiIndex) const
{
	return data.unfoldSubSliceIndex(inverseSubSliceIndex(colIndices, multiIndex));
}

size_t DataCube::flatSubSliceIndex(const SeriesList &colIndices, const MultiIndex &multiIndex) const
{
	return data.unfoldSubSliceIndex(subSliceIndex(colIndices, multiIndex));
}

CategoryMap DataCube::categories(
    const MultiDim::MultiIndex &index) const
{
	CategoryMap res;

	for (auto i = 0u; i < index.size(); i++)
	{
		auto series = getSeriesByDim(MultiDim::DimIndex{i});

		auto colIndex = series.getColIndex();

		auto value =
		    table->getInfo(colIndex).discreteValues()[index[i]];

		res.insert({series.toString(*table), value});
	}
	return res;
}

ValueMap DataCube::values(const MultiDim::MultiIndex &index) const
{
	ValueMap res;

	const auto &cell = data.at(index);

	for (auto i = 0u; i < cell.subCells.size(); i++)
	{
		auto series = getSeriesBySubIndex(SubCellIndex{i});

		auto value = (double)cell.subCells[i];

		res.insert({series.toString(*table), value});
	}
	return res;
}

CellInfo DataCube::cellInfo(const MultiDim::MultiIndex &index) const
{
	if (!table) return CellInfo();

	return CellInfo{ categories(index), values(index) };
}

MultiDim::SubSliceIndex DataCube::subSliceIndex(
    const CategoryMap &categories) const
{
	MultiDim::SubSliceIndex index;
	for (auto &pair : categories)
	{
		auto colIdx = table->getColumn(pair.first);
		auto seriesIdx = table->getIndex(colIdx);
		auto &values = table->getInfo(colIdx).discreteValueIndexes();
		auto valIdx = values.at(pair.second);
		auto dimIdx = getDimBySeries(SeriesIndex(seriesIdx));
		index.push_back(MultiDim::SliceIndex{dimIdx, MultiDim::Index{valIdx}});
	}
	return index;
}