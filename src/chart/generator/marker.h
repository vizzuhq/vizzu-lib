#ifndef CHART_GENERATOR_MARKER_H
#define CHART_GENERATOR_MARKER_H

#include <optional>

#include "base/anim/interpolated.h"
#include "base/conv/auto_json.h"
#include "base/geom/circle.h"
#include "base/geom/point.h"
#include "base/gfx/color.h"
#include "base/math/fuzzybool.h"
#include "chart/options/options.h"
#include "dataframe/old/types.h"

#include "colorbase.h"

namespace Vizzu::Gen
{

class ChannelsStats;

class Marker
{
public:
	using MarkerIndex = Options::MarkerIndex;

	Marker(const Options &options,
	    const Data::DataCube &data,
	    ChannelsStats &stats,
	    const Data::MultiIndex &index,
	    MarkerIndex idx,
	    bool needMarkerInfo,
	    bool rectangleSpacing);

	::Anim::Interpolated<ColorBase> colorBase;
	Geom::Point position;
	Geom::Point size;
	Geom::Point spacing;
	double sizeFactor;
	Math::FuzzyBool enabled;
	std::shared_ptr<const Data::CellInfo> cellInfo;

	struct Label
	{
		std::optional<double> value;
		std::string indexStr;
		bool operator==(const Label &other) const;
		[[nodiscard]] bool hasValue() const
		{
			return value.has_value();
		}
	};

	::Anim::Interpolated<Label> label;

	using Id = Data::MarkerId;

	::Anim::Interpolated<Id> mainId;
	Id subId;
	Id sizeId;

	MarkerIndex idx;
	::Anim::Interpolated<MarkerIndex> prevMainMarkerIdx;
	::Anim::Interpolated<MarkerIndex> nextMainMarkerIdx;
	::Anim::Interpolated<MarkerIndex> nextSubMarkerIdx;

	void setNextMarker(bool first,
	    Marker &marker,
	    bool horizontal,
	    bool main);
	void resetSize(bool horizontal);

	[[nodiscard]] Geom::Rect toRectangle() const;
	void fromRectangle(const Geom::Rect &rect);

	[[nodiscard]] Math::Range<double> getSizeBy(
	    bool horizontal) const;
	void setSizeBy(bool horizontal, Math::Range<double> range);

	void setIdOffset(size_t offset);
	Conv::JSONObj &&appendToJSON(Conv::JSONObj &&jsonObj) const;

private:
	double getValueForChannel(const Channels &channels,
	    ChannelId type,
	    const Data::DataCube &data,
	    ChannelsStats &stats,
	    const Data::MultiIndex &index,
	    const Data::MarkerId * = nullptr) const;
};

}

#endif
