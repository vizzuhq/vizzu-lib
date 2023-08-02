#ifndef PLOT_H
#define PLOT_H

#include <array>
#include <memory>
#include <optional>
#include <unordered_map>

#include "chart/main/style.h"
#include "chart/options/options.h"
#include "data/table/datatable.h"

#include "axis.h"
#include "channelstats.h"
#include "guides.h"
#include "marker.h"

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
	using Buckets =
	    std::unordered_map<uint64_t, std::map<uint64_t, uint64_t>>;
	using CellInfo = std::vector<std::pair<std::string, std::string>>;
	using Markers = std::vector<Marker>;

	struct MarkerInfoContent
	{
		std::optional<Options::MarkerId> markerId;
		CellInfo content;

		MarkerInfoContent();
		MarkerInfoContent(const Marker &marker,
		    Data::DataCube *dataCube = nullptr);
		operator bool() const;
		bool operator==(const MarkerInfoContent &op) const;
	};

	using MarkerInfo = ::Anim::Interpolated<MarkerInfoContent>;
	using MarkersInfo = std::map<uint64_t, MarkerInfo>;

	static bool dimensionMatch(const Plot &a, const Plot &b);

	Math::FuzzyBool anySelected;
	Math::FuzzyBool anyAxisSet;
	MeasureAxises measureAxises;
	Guides guides;
	DimensionAxises dimensionAxises;
	Math::FuzzyBool keepAspectRatio;

	Plot(const Plot &other) = default;
	Plot(PlotOptionsPtr options, const Plot &other);
	Plot(const Data::DataTable &dataTable,
	    PlotOptionsPtr opts,
	    Styles::Chart style,
	    bool setAutoParams = true);
	[[nodiscard]] const Markers &getMarkers() const
	{
		return markers;
	}
	Markers &getMarkers() { return markers; }
	void prependMarkers(const Plot &plot, bool enabled);
	void appendMarkers(const Plot &plot, bool enabled);
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
		return dataCube;
	}
	[[nodiscard]] const ChannelsStats &getStats() const
	{
		return stats;
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
	Data::DataCube dataCube;
	ChannelsStats stats;
	Markers markers;
	MarkersInfo markersInfo;

	Buckets mainBuckets;
	Buckets subBuckets;

	void generateMarkers(const Data::DataCube &dataCube,
	    const Data::DataTable &table);
	void generateMarkersInfo();
	void linkMarkers(const Buckets &buckets, bool main);
	void normalizeXY();
	void calcAxises(const Data::DataTable &dataTable);
	MeasureAxis calcAxis(ChannelId type, const Data::DataTable &dataTable);
	void calcDimensionAxises(const Data::DataTable &table);
	void calcDimensionAxis(ChannelId type,
	    const Data::DataTable &table);
	void addAlignment();
	void addSeparation();
	void normalizeSizes();
	void normalizeColors();
	std::vector<std::pair<uint64_t, double>>
	sortedBuckets(const Buckets &buckets, bool main);
	void clearEmptyBuckets(const Buckets &buckets, bool main);
};

using PlotPtr = std::shared_ptr<Plot>;

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
