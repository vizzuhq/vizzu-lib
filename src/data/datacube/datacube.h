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
	using MultiIndex = MultiDim::MultiIndex;

	struct Id
	{
		using SubSliceIndex = MultiDim::SubSliceIndex;
		using SliceIndex = MultiDim::SliceIndex;
		SubSliceIndex itemSliceIndex;
		uint64_t seriesId{};
		uint64_t itemId{};

		bool operator==(const Id &) const = default;
	};

	DataCube(const DataTable &table,
	    const DataCubeOptions &options,
	    const Filter &filter = Filter());

	[[nodiscard]] const Data &getData() const { return data; }
	[[nodiscard]] const DataTable *getTable() const { return table; }
	[[nodiscard]] MultiDim::DimIndex getDimBySeries(
	    const SeriesIndex &index) const;
	[[nodiscard]] SeriesIndex getSeriesByDim(
	    MultiDim::DimIndex index) const;
	[[nodiscard]] SeriesIndex getSeriesBySubIndex(
	    SubCellIndex index) const;

	[[nodiscard]] size_t combinedIndexOf(const SeriesList &colIndices,
	    MultiIndex multiIndex) const;

	[[nodiscard]] size_t combinedSizeOf(
	    const SeriesList &colIndices) const;

	[[nodiscard]] Aggregator aggregateAt(const MultiIndex &multiIndex,
	    const SeriesList &sumCols,
	    SeriesIndex seriesId) const;

	[[nodiscard]] Aggregator valueAt(const MultiIndex &multiIndex,
	    const SeriesIndex &seriesId) const;

	[[nodiscard]] size_t subSliceID(const SeriesList &colIndices,
	    const MultiIndex &multiIndex) const;

	[[nodiscard]] size_t flatSubSliceIndex(
	    const SeriesList &colIndices,
	    const MultiIndex &multiIndex) const;

	[[nodiscard]] Id::SubSliceIndex subSliceIndex(
	    const SeriesList &colIndices,
	    MultiIndex multiIndex) const;

	[[nodiscard]] size_t subCellSize() const;

	[[nodiscard]] bool empty() const;

	[[nodiscard]] CellInfo::Values values(
	    const MultiIndex &index) const;
	[[nodiscard]] CellInfo::Categories categories(
	    const MultiIndex &index) const;
	[[nodiscard]] CellInfo cellInfo(const MultiIndex &index) const;

	[[nodiscard]] Id getId(const SeriesList &dimensionIds,
	    const MultiIndex &index) const;

	[[nodiscard]] std::string getValue(Id::SliceIndex index,
	    std::string def = "") const;

private:
	Data data;
	const DataTable *table{};
	std::map<SeriesIndex, MultiDim::DimIndex> dimBySeries;
	std::vector<SeriesIndex> seriesByDim;

	std::map<SeriesIndex, SubCellIndex> subIndexBySeries;
	std::vector<SeriesIndex> seriesBySubIndex;

	static MultiIndex getIndex(const DataTable::Row &row,
	    const std::set<SeriesIndex> &indices,
	    size_t rowIndex);

	[[nodiscard]] Id::SubSliceIndex inverseSubSliceIndex(
	    const SeriesList &colIndices,
	    MultiIndex multiIndex) const;
};

}

#endif
