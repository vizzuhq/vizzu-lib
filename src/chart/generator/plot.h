#ifndef PLOT_H
#define PLOT_H

#include <array>
#include <memory>
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
	typedef std::unordered_map<uint64_t, std::map<uint64_t, uint64_t>>
	    Buckets;
	typedef std::vector<std::pair<std::string, std::string>> CellInfo;
	typedef std::vector<Marker> Markers;

	struct MarkerInfoContent
	{
		Options::MarkerId markerId;
		CellInfo content;

		MarkerInfoContent();
		MarkerInfoContent(const Marker &marker,
		    Data::DataCube *dataCube = nullptr);
		operator bool() const;
		bool operator==(const MarkerInfoContent &op) const;
	};

	typedef ::Anim::Interpolated<MarkerInfoContent> MarkerInfo;
	typedef std::map<Options::MarkerId, MarkerInfo> MarkersInfo;

	static bool dimensionMatch(const Plot &a, const Plot &b);

	Math::FuzzyBool anySelected;
	Math::FuzzyBool anyAxisSet;
	Axises axises;
	Guides guides;
	DimensionAxises dimensionAxises;
	Math::FuzzyBool keepAspectRatio;

	Plot(const Plot &other) = default;
	Plot(PlotOptionsPtr options, const Plot &other);
	Plot(const Data::DataTable &dataTable,
	    PlotOptionsPtr opts,
	    Styles::Chart style,
	    bool setAutoParams = true);
	const Markers &getMarkers() const { return markers; }
	Markers &getMarkers() { return markers; }
	void prependMarkers(const Plot &plot, bool enabled);
	void appendMarkers(const Plot &plot, bool enabled);
	const MarkersInfo &getMarkersInfo() const { return markersInfo; }
	MarkersInfo &getMarkersInfo() { return markersInfo; }
	PlotOptionsPtr getOptions() const { return options; }
	const Data::DataCube &getDataCube() const { return dataCube; }
	const ChannelsStats &getStats() const { return stats; }
	const Styles::Chart &getStyle() const { return style; }
	Styles::Chart &getStyle() { return style; }
	const Data::DataTable &getTable() const { return dataTable; };
	void detachOptions();
	bool isEmpty() const;

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
	Axis calcAxis(ChannelId type, const Data::DataTable &dataTable);
	void calcDimensionAxises(const Data::DataTable &table);
	void calcDimensionAxis(ChannelId type, const Data::DataTable &table);
	void addAlignment();
	void addSeparation();
	void normalizeSizes();
	void normalizeColors();
	void recalcStackedLineChart();
	std::vector<std::pair<uint64_t, double>>
	sortedBuckets(const Buckets &buckets, bool main);
	void clearEmptyBuckets(const Buckets &buckets, bool main);
};

typedef std::shared_ptr<Plot> PlotPtr;

struct PlotParent
{
	virtual ~PlotParent() {}
	virtual PlotPtr getPlot() const = 0;
};

Plot::MarkersInfo interpolate(const Plot::MarkersInfo &op1,
    const Plot::MarkersInfo &op2,
    double factor);

}
}

#endif
