#ifndef PLOTBUILDER_H
#define PLOTBUILDER_H

#include "chart/main/style.h"
#include "chart/options/options.h"
#include "dataframe/old/datatable.h"

#include "plotptr.h"

namespace Vizzu::Gen
{
class PlotBuilder
{
public:
	PlotBuilder(const Data::DataTable &dataTable,
	    const PlotOptionsPtr &opts,
	    const Styles::Chart &style);

	PlotPtr &&build() && { return std::move(plot); }

private:
	Data::DataCube dataCube;
	PlotPtr plot;

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
	[[nodiscard]] std::vector<std::pair<double, std::size_t>>
	sortedBuckets(const Buckets &buckets, bool main) const;
	void clearEmptyBuckets(const Buckets &buckets, bool main) const;
	void addSpecLayout(Buckets &buckets);

	Math::Range<double> &getMeasTrackRange(ChannelId type) const;
};
}

#endif // PLOTBUILDER_H
