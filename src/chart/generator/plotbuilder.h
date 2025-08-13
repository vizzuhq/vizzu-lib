#ifndef PLOTBUILDER_H
#define PLOTBUILDER_H

#include "chart/main/style.h"
#include "chart/options/options.h"
#include "dataframe/old/datatable.h"

#include "axis.h"
#include "plotptr.h"

namespace Vizzu::Gen
{

struct Buckets;

class PlotBuilder
{
public:
	PlotBuilder(const Data::DataTable &dataTable,
	    const PlotOptionsPtr &options,
	    const Styles::Chart &style);

	PlotPtr &&build() && { return std::move(plot); }

private:
	Data::DataCube dataCube;
	PlotPtr plot;
	ChannelStats stats;

	struct BucketSortInfo
	{
		std::size_t index{};
		double size{};
		std::optional<std::string> label{};
	};

	struct BucketSeparationInfo
	{
		std::size_t itemId{};
		std::vector<Data::SliceIndex> index{};
		Math::Range<> containsValues{0.0, 0.0};
		Math::Range<> atRange{0.0, 0.0};

		bool operator<(const std::size_t &id) const noexcept
		{
			return itemId < id;
		}
	};

	void initDimensionTrackers();
	Buckets generateMarkers();
	void linkMarkers(Buckets &buckets);
	[[nodiscard]] bool linkMarkers(const Buckets &buckets,
	    AxisId axisIndex) const;
	void calcAxises(const Data::DataTable &dataTable,
	    Buckets &buckets);
	void calcLegendAndLabel(const Data::DataTable &dataTable);
	void calcAxis(const Data::DataTable &dataTable, AxisId type);
	void addAlignment(const Buckets &buckets, AxisId axisIndex) const;
	[[nodiscard]] std::vector<BucketSeparationInfo>
	addSeparation(const Buckets &buckets, AxisId axisIndex);
	void normalizeSizes();
	void normalizeColors();
	[[nodiscard]] std::vector<BucketSortInfo>
	sortedBuckets(const Buckets &buckets, AxisId axisIndex) const;
	void addSpecLayout(Buckets &buckets);
	void addAxisLayout(Buckets &buckets,
	    const Data::DataTable &dataTable);
};
}

#endif // PLOTBUILDER_H
