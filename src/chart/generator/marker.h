#ifndef CHART_GENERATOR_MARKER_H
#define CHART_GENERATOR_MARKER_H

#include <optional>

#include "base/anim/interpolated.h"
#include "base/geom/circle.h"
#include "base/geom/point.h"
#include "base/gfx/color.h"
#include "base/math/fuzzybool.h"
#include "base/util/eventdispatcher.h"
#include "chart/main/style.h"
#include "chart/options/options.h"
#include "data/datacube/datacube.h"

#include "colorbuilder.h"

namespace Vizzu::Gen
{

class ChannelsStats;

class Marker : public Util::EventTarget
{
public:
	Marker(const Options &options,
	    const Styles::Chart &style,
	    const Data::DataCube &data,
	    const Data::DataTable &table,
	    ChannelsStats &stats,
	    const Data::MultiDim::MultiIndex &index,
	    size_t idx);

	Data::MultiDim::MultiIndex index;
	Gfx::Color color;
	ColorBuilder colorBuilder;
	Geom::Point position;
	Geom::Point size;
	Geom::Point spacing;
	double sizeFactor;
	Math::FuzzyBool enabled;
	Math::FuzzyBool selected;
	Data::CellInfo cellInfo;

	struct Label
	{
		std::optional<double> value;
		std::optional<Data::ColumnIndex> measureId;
		std::string unit;
		std::string indexStr;
		Label() = default;
		Label(const Data::MultiDim::SubSliceIndex &index,
		    const Data::DataCube &data,
		    const Data::DataTable &table);
		Label(double value,
		    const Data::SeriesIndex &measure,
		    const Data::MultiDim::SubSliceIndex &index,
		    const Data::DataCube &data,
		    const Data::DataTable &table);
		bool operator==(const Label &other) const;
		[[nodiscard]] bool hasValue() const
		{
			return value.has_value();
		}
		static std::string getIndexString(
		    const Data::MultiDim::SubSliceIndex &index,
		    const Data::DataCube &data,
		    const Data::DataTable &table);
	};

	::Anim::Interpolated<Label> label;

	struct Id
	{
		uint64_t seriesId;
		Data::MultiDim::SubSliceIndex itemSliceIndex;
		uint64_t itemId;
		Id() = default;
		bool operator==(const Id &other) const = default;
		Id(const Data::DataCube &,
		    const Channel::DimensionIndices &dimensionIds,
		    const Data::MultiDim::MultiIndex &);
	};

	::Anim::Interpolated<Id> mainId;
	Id subId;
	Id sizeId;
	Id stackId;

	uint64_t idx;
	::Anim::Interpolated<uint64_t> prevMainMarkerIdx;
	::Anim::Interpolated<uint64_t> nextMainMarkerIdx;
	::Anim::Interpolated<uint64_t> nextSubMarkerIdx;

	void setNextMarker(uint64_t itemId,
	    Marker *marker,
	    bool horizontal,
	    bool main);
	void resetSize(bool horizontal);

	[[nodiscard]] Geom::Rect toRectangle() const;
	void fromRectangle(const Geom::Rect &rect);

	[[nodiscard]] Math::Range<double> getSizeBy(
	    bool horizontal) const;
	void setSizeBy(bool horizontal, Math::Range<double> range);

	void setIdOffset(size_t offset);
	[[nodiscard]] std::string toJson(
	    ) const override;

private:
	const Data::DataTable *table;

	double getValueForChannel(const Channels &channels,
	    ChannelId type,
	    const Data::DataCube &data,
	    ChannelsStats &stats,
	    const Channel *subChannel = nullptr,
	    bool inhibitStack = false) const;
};

}

#endif
