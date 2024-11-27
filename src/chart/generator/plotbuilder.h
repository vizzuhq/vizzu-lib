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

	struct BucketInfo
	{
		std::size_t index{};
		double size{};
	};

	void initDimensionTrackers();
	Buckets generateMarkers(std::size_t &mainBucketSize);
	void linkMarkers(Buckets &subBuckets);
	[[nodiscard]] bool linkMarkers(const Buckets &buckets,
	    AxisId axisIndex) const;
	void calcAxises(const Data::DataTable &dataTable);
	void calcLegendAndLabel(const Data::DataTable &dataTable);
	void calcAxis(const Data::DataTable &dataTable, AxisId type);
	void addAlignment(const Buckets &subBuckets) const;
	void addSeparation(const Buckets &subBuckets,
	    const std::size_t &mainBucketSize) const;
	void normalizeSizes();
	void normalizeColors();
	[[nodiscard]] std::vector<BucketInfo>
	sortedBuckets(const Buckets &buckets, AxisId axisIndex) const;
	void addSpecLayout(Buckets &buckets);
	void addAxisLayout(Buckets &subBuckets,
	    const std::size_t &mainBucketSize,
	    const Data::DataTable &dataTable);
};
}

#endif // PLOTBUILDER_H
