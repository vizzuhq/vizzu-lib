#ifndef PLOT_H
#define PLOT_H

#include <array>
#include <memory>
#include <optional>

#include "chart/main/style.h"
#include "chart/options/options.h"
#include "dataframe/old/datatable.h"

#include "axis.h"
#include "channelstats.h"
#include "guides.h"
#include "marker.h"
#include "plotptr.h"

namespace Vizzu
{

namespace Anim
{
class Keyframe;
namespace Morph
{
class AbstractMorph;
}
}

namespace Gen
{

class Selector;

class Plot
{
	friend class Anim::Keyframe;
	friend class Anim::Morph::AbstractMorph;
	friend class Selector;

public:
	using Markers = std::vector<Marker>;

	struct MarkerInfoContent
	{
		std::optional<Options::MarkerId> markerId;
		std::shared_ptr<
		    const std::vector<std::pair<std::string, std::string>>>
		    info;

		MarkerInfoContent() = default;
		explicit MarkerInfoContent(const Marker &marker);
		explicit operator bool() const;
		bool operator==(const MarkerInfoContent &op) const;
	};

	using MarkerInfo = ::Anim::Interpolated<MarkerInfoContent>;
	using MarkersInfo = std::map<uint64_t, MarkerInfo>;

	static bool dimensionMatch(const Plot &a, const Plot &b);

	Math::FuzzyBool anyAxisSet;
	CommonAxises commonAxises;
	MeasureAxises measureAxises;
	Guides guides;
	DimensionAxises dimensionAxises;
	Math::FuzzyBool keepAspectRatio;
	std::optional<Orientation> markerConnectionOrientation;

	Plot(const Plot &other) = default;
	Plot(PlotOptionsPtr options, const Plot &other);
	Plot(const Data::DataTable &dataTable,
	    PlotOptionsPtr opts,
	    Styles::Chart style);
	[[nodiscard]] const Markers &getMarkers() const
	{
		return markers;
	}
	Markers &getMarkers() { return markers; }
	void prependMarkers(const Plot &plot);
	void appendMarkers(const Plot &plot);
	[[nodiscard]] const MarkersInfo &getMarkersInfo() const
	{
		return markersInfo;
	}
	MarkersInfo &getMarkersInfo() { return markersInfo; }
	[[nodiscard]] const PlotOptionsPtr &getOptions() const
	{
		return options;
	}
	[[nodiscard]] const Data::DataCube &getDataCube() const
	{
		return *dataCube;
	}
	[[nodiscard]] const Styles::Chart &getStyle() const
	{
		return style;
	}
	Styles::Chart &getStyle() { return style; }
	[[nodiscard]] const Data::DataTable &getTable() const
	{
		return dataTable;
	};
	void detachOptions();
	[[nodiscard]] bool isEmpty() const;

private:
	const Data::DataTable &dataTable;
	PlotOptionsPtr options;
	Styles::Chart style;
	std::optional<Data::DataCube> dataCube;
	ChannelsStats *stats;
	Markers markers;
	MarkersInfo markersInfo;

	std::size_t mainBucketSize{};

	[[nodiscard]] ChannelsStats &getStats() { return *stats; }
	Buckets generateMarkers();
	[[nodiscard]] bool linkMarkers(const Buckets &buckets,
	    bool main) const;
	void normalizeXY();
	void calcMeasureAxises();
	void calcMeasureAxis(ChannelId type);
	void calcDimensionAxises();
	void calcDimensionAxis(ChannelId type);
	void addAlignment(const Buckets &subBuckets) const;
	void addSeparation(const Buckets &subBuckets) const;
	void normalizeSizes();
	void normalizeColors();
	[[nodiscard]] std::vector<std::pair<double, uint64_t>>
	sortedBuckets(const Buckets &buckets, bool main) const;
	void clearEmptyBuckets(const Buckets &buckets, bool main) const;
};

struct PlotParent
{
	virtual ~PlotParent() = default;
	[[nodiscard]] virtual PlotPtr getPlot() const = 0;
};

Plot::MarkersInfo interpolate(const Plot::MarkersInfo &op1,
    const Plot::MarkersInfo &op2,
    double factor);

}
}

#endif
