#ifndef DATACUBE_H
#define DATACUBE_H

#include <map>
#include <string>
#include <vector>

#include "data/multidim/multidimarray.h"

#include "aggregator.h"
#include "datacubecell.h"
#include "datacubeoptions.h"
#include "datafilter.h"

namespace Vizzu::Data
{

class DataTable;
template <typename T> class TableRow;

using SubCellIndex =
    Type::UniqueType<uint64_t, struct SubCellIndexTypeId>;

struct CellInfo
{
	using Categories =
	    std::vector<std::pair<Data::SeriesIndex, uint64_t>>;
	using Values = std::vector<std::pair<Data::SeriesIndex, double>>;
	Categories categories;
	Values values;
};

using MarkerIdStrings =
    std::vector<std::pair<std::string, std::string>>;

class DataCube
{
public:
	using Data = MultiDim::Array<DataCubeCell>;

	DataCube() : table(nullptr) {}

	DataCube(const DataTable &table,
	    const DataCubeOptions &options,
	    const Filter &filter = Filter());

	[[nodiscard]] const Data &getData() const { return data; }
	[[nodiscard]] const DataTable *getTable() const { return table; }
	[[nodiscard]] MultiDim::DimIndex getDimBySeries(
	    SeriesIndex index) const;
	[[nodiscard]] SeriesIndex getSeriesByDim(
	    MultiDim::DimIndex index) const;
	[[nodiscard]] SeriesIndex getSeriesBySubIndex(
	    SubCellIndex index) const;

	[[nodiscard]] size_t combinedIndexOf(const SeriesList &colIndices,
	    MultiDim::MultiIndex multiIndex) const;

	[[nodiscard]] size_t combinedSizeOf(
	    const SeriesList &colIndices) const;

	[[nodiscard]] Aggregator aggregateAt(
	    const MultiDim::MultiIndex &multiIndex,
	    const SeriesList &sumCols,
	    SeriesIndex seriesId) const;

	[[nodiscard]] double sumTillAt(const SeriesList &colIndices,
	    const SeriesList &sumCols,
	    const MultiDim::MultiIndex &multiIndex,
	    SeriesIndex seriesId) const;

	[[nodiscard]] Aggregator valueAt(
	    const MultiDim::MultiIndex &multiIndex,
	    const SeriesIndex &seriesId) const;

	[[nodiscard]] size_t subSliceID(const SeriesList &colIndices,
	    const MultiDim::MultiIndex &multiIndex) const;

	[[nodiscard]] size_t flatSubSliceIndex(
	    const SeriesList &colIndices,
	    const MultiDim::MultiIndex &multiIndex) const;

	[[nodiscard]] MultiDim::SubSliceIndex subSliceIndex(
	    const SeriesList &colIndices,
	    MultiDim::MultiIndex multiIndex) const;

	[[nodiscard]] size_t subCellSize() const;

	[[nodiscard]] bool empty() const;

	[[nodiscard]] CellInfo::Values values(
	    const MultiDim::MultiIndex &index) const;
	[[nodiscard]] CellInfo::Categories categories(
	    const MultiDim::MultiIndex &index) const;
	[[nodiscard]] CellInfo cellInfo(
	    const MultiDim::MultiIndex &index) const;
	[[nodiscard]] MultiDim::SubSliceIndex subSliceIndex(
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

	[[nodiscard]] MultiDim::SubSliceIndex inverseSubSliceIndex(
	    const SeriesList &colIndices,
	    MultiDim::MultiIndex multiIndex) const;
};

}

#endif
