#ifndef DATACUBE_H
#define DATACUBE_H

#include <list>
#include <map>
#include <string>
#include <vector>

#include "data/multidim/multidimarray.h"

#include "aggregator.h"
#include "datacubecell.h"
#include "datacubeoptions.h"
#include "datafilter.h"

namespace Vizzu
{
namespace Data
{

class DataTable;
template <typename T> class TableRow;

typedef Type::UniqueType<uint64_t, struct SubCellIndexTypeId>
    SubCellIndex;

struct CellInfo
{
	typedef std::vector<std::pair<Data::SeriesIndex, uint64_t>>
	    Categories;
	typedef std::vector<std::pair<Data::SeriesIndex, double>> Values;
	Categories categories;
	Values values;
};

typedef std::vector<std::pair<std::string, std::string>>
    MarkerIdStrings;

class DataCube
{
public:
	typedef MultiDim::Array<DataCubeCell> Data;

	DataCube() : table(nullptr) {}

	DataCube(const DataTable &table,
	    const DataCubeOptions &options,
	    const Filter &filter = Filter());

	const Data &getData() const { return data; }
	const DataTable *getTable() const { return table; }
	MultiDim::DimIndex getDimBySeries(SeriesIndex index) const;
	SeriesIndex getSeriesByDim(MultiDim::DimIndex index) const;
	SeriesIndex getSeriesBySubIndex(SubCellIndex index) const;

	size_t combinedIndexOf(const SeriesList &colIndices,
	    MultiDim::MultiIndex multiIndex) const;

	size_t combinedSizeOf(const SeriesList &colIndices) const;

	Aggregator aggregateAt(const MultiDim::MultiIndex &multiIndex,
	    const SeriesList &sumCols,
	    SeriesIndex seriesId) const;

	double sumTillAt(const SeriesList &colIndices,
	    const SeriesList &sumCols,
	    const MultiDim::MultiIndex &multiIndex,
	    SeriesIndex seriesId) const;

	Aggregator valueAt(const MultiDim::MultiIndex &multiIndex,
	    const SeriesIndex &seriesId) const;

	size_t subSliceID(const SeriesList &colIndices,
	    const MultiDim::MultiIndex &multiIndex) const;

	size_t flatSubSliceIndex(const SeriesList &colIndices,
	    const MultiDim::MultiIndex &multiIndex) const;

	MultiDim::SubSliceIndex subSliceIndex(
	    const SeriesList &colIndices,
	    MultiDim::MultiIndex multiIndex) const;

	size_t subCellSize() const;

	bool empty() const;

	CellInfo::Values values(const MultiDim::MultiIndex &index) const;
	CellInfo::Categories categories(
	    const MultiDim::MultiIndex &index) const;
	CellInfo cellInfo(const MultiDim::MultiIndex &index) const;
	MultiDim::SubSliceIndex subSliceIndex(
	    const MarkerIdStrings &stringMarkerId) const;

private:
	Data data;
	const DataTable *table;
	std::map<SeriesIndex, MultiDim::DimIndex> dimBySeries;
	std::vector<SeriesIndex> seriesByDim;

	std::map<SeriesIndex, SubCellIndex> subIndexBySeries;
	std::vector<SeriesIndex> seriesBySubIndex;

	static MultiDim::MultiIndex getIndex(const TableRow<double> &row,
	    const std::vector<SeriesIndex> &indices,
	    size_t rowIndex);

	MultiDim::SubSliceIndex inverseSubSliceIndex(
	    const SeriesList &colIndices,
	    MultiDim::MultiIndex multiIndex) const;
};

}
}

#endif
