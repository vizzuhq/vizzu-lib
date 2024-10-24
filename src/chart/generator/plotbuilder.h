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
	[[nodiscard]] bool linkMarkers(const Buckets &buckets,
	    bool main) const;
	void normalizeXY();
	void calcMeasureAxises(const Data::DataTable &dataTable);
	void calcMeasureAxis(const Data::DataTable &dataTable,
	    ChannelId type);
	void calcDimensionAxises();
	void calcDimensionAxis(ChannelId type);
	void addAlignment(const Buckets &subBuckets) const;
	void addSeparation(const Buckets &subBuckets,
	    const std::size_t &mainBucketSize) const;
	void normalizeSizes();
	void normalizeColors();
	[[nodiscard]] std::vector<BucketInfo>
	sortedBuckets(const Buckets &buckets, bool main) const;
	void addSpecLayout(Buckets &buckets);
};
}

#endif // PLOTBUILDER_H
