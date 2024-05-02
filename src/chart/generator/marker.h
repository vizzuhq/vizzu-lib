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
	Marker(const Options &options,
	    const Data::DataCube &data,
	    ChannelsStats &stats,
	    const Data::MultiIndex &index,
	    size_t idx);

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
		std::string measureId;
		std::string unit;
		std::string indexStr;
		Label() = default;
		explicit Label(std::string &&indexStr);
		Label(double value,
		    const Data::SeriesIndex &measure,
		    const Data::DataCube &data,
		    std::string &&indexStr);
		bool operator==(const Label &other) const;
		[[nodiscard]] bool hasValue() const
		{
			return value.has_value();
		}
		static std::string getIndexString(const Data::MarkerId &);
	};

	::Anim::Interpolated<Label> label;

	using Id = Data::MarkerId;

	::Anim::Interpolated<Id> mainId;
	Id subId;
	Id sizeId;
	Id stackId;

	uint64_t idx;
	::Anim::Interpolated<uint64_t> prevMainMarkerIdx;
	::Anim::Interpolated<uint64_t> nextMainMarkerIdx;
	::Anim::Interpolated<uint64_t> nextSubMarkerIdx;

	void setNextMarker(uint64_t itemId,
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
	[[nodiscard]] std::string toJSON() const;

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
