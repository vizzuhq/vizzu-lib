#ifndef PLOT_H
#define PLOT_H

#include <array>
#include <memory>
#include <optional>

#include "chart/main/style.h"
#include "chart/options/options.h"
#include "dataframe/old/datatable.h"

#include "axis.h"
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

class Plot
{
	friend class PlotBuilder;

public:
	using Markers = std::vector<Marker>;
	using MarkerInfoId = Options::MarkerInfoId;

	struct MarkerInfoContent
	{
		std::optional<Marker::MarkerIndexPosition> markerId;
		std::shared_ptr<
		    const std::vector<std::pair<std::string, std::string>>>
		    info;

		MarkerInfoContent() = default;
		explicit MarkerInfoContent(const Marker &marker);
		explicit operator bool() const;
		bool operator==(const MarkerInfoContent &op) const;
	};

	using MarkerInfo = ::Anim::Interpolated<MarkerInfoContent>;
	using MarkersInfo = std::map<MarkerInfoId, MarkerInfo>;

	Axises axises;
	Guides guides;
	Math::FuzzyBool keepAspectRatio;
	std::optional<Orientation> markerConnectionOrientation;

	Plot(const Plot &other) = default;
	Plot(PlotOptionsPtr opts, Styles::Chart style);
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
	[[nodiscard]] const Styles::Chart &getStyle() const
	{
		return style;
	}
	Styles::Chart &getStyle() { return style; }
	void detachOptions();
	[[nodiscard]] bool isEmpty() const;

	static bool dimensionMatch(const Plot &a, const Plot &b);
	static bool hasMarkerChange(const Plot &source,
	    const Plot &target);
	static void mergeMarkersAndCellInfo(Plot &source, Plot &target);

private:
	PlotOptionsPtr options;
	Styles::Chart style;
	Markers markers;
	MarkersInfo markersInfo;
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
