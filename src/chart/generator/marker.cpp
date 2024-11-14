#include "marker.h"

#include <cstdint>
#include <optional>
#include <utility>

#include "base/conv/auto_json.h"
#include "base/geom/point.h"
#include "base/geom/rect.h"
#include "base/math/range.h"
#include "base/refl/auto_enum.h"
#include "chart/options/align.h"
#include "chart/options/channel.h"
#include "chart/options/coordsystem.h"
#include "chart/options/shapetype.h"
#include "dataframe/old/datatable.h"
#include "dataframe/old/types.h"

#include "axis.h"

namespace Vizzu::Gen
{

Marker::Marker(const Options &options,
    const Data::DataCube &data,
    ChannelStats &stats,
    const Data::SeriesList &mainAxisList,
    const Data::SeriesList &subAxisList,
    const Data::MultiIndex &index,
    bool needMarkerInfo) :
    cellInfo(data.cellInfo(index, needMarkerInfo)),
    sizeId(data.getId(options.getChannels()
                          .at(ChannelId::size)
                          .dimensionsWithLevel(),
        index)),
    idx{index.marker_id}
{
	const auto &channels = options.getChannels();
	auto color = getValueForChannel(channels,
	    ChannelId::color,
	    data,
	    stats,
	    index);

	auto lightness = getValueForChannel(channels,
	    ChannelId::lightness,
	    data,
	    stats,
	    index);

	colorBase = channels.at(ChannelId::color).isDimension()
	              ? ColorBase(static_cast<uint32_t>(color), lightness)
	              : ColorBase(color, lightness);

	sizeFactor = getValueForChannel(channels,
	    ChannelId::size,
	    data,
	    stats,
	    index,
	    &sizeId);

	subId = data.getId({subAxisList, options.subAxis().labelLevel},
	    index);
	auto *subAxisId = subAxisList == options.subAxis().dimensions()
	                    ? &subId
	                    : nullptr;
	mainId = data.getId({mainAxisList, options.mainAxis().labelLevel},
	    index);

	auto horizontal = options.isHorizontal();
	auto lineOrCircle = options.geometry == ShapeType::line
	                 || options.geometry == ShapeType::circle;
	auto polar = options.coordSystem.get() == CoordSystem::polar;

	position.x = size.x = getValueForChannel(channels,
	    ChannelId::x,
	    data,
	    stats,
	    index,
	    horizontal ? &mainId : subAxisId);

	auto yChannelRectDim =
	    channels.at(AxisId::y).isDimension()
	    && channels.at(AxisId::y).hasDimension()
	    && options.geometry == ShapeType::rectangle
	    && options.align != Base::Align::Type::stretch;

	spacing.x =
	    (horizontal || (lineOrCircle && !polar) || yChannelRectDim)
	            && options.getChannels().anyAxisSet()
	            && channels.at(AxisId::x).isDimension()
	        ? 1
	        : 0;

	position.y = size.y = getValueForChannel(channels,
	    ChannelId::y,
	    data,
	    stats,
	    index,
	    !horizontal ? &mainId : subAxisId);

	auto xChannelRectDim =
	    channels.at(AxisId::x).isDimension()
	    && channels.at(AxisId::x).hasDimension()
	    && options.geometry == ShapeType::rectangle
	    && options.align != Base::Align::Type::stretch;

	spacing.y = (!horizontal || lineOrCircle || xChannelRectDim)
	                 && options.getChannels().anyAxisSet()
	                 && channels.at(AxisId::y).isDimension()
	              ? 1
	              : 0;

	if (auto &&labelChannel = channels.at(ChannelId::label);
	    !labelChannel.isEmpty()) {
		auto &&value = std::make_optional(getValueForChannel(channels,
		    ChannelId::label,
		    data,
		    stats,
		    index));

		label =
		    Label{labelChannel.isDimension() ? std::nullopt : value,
		        data.joinDimensionValues(labelChannel.dimensions(),
		            index)};
	}
}

bool Marker::connectMarkers(bool first,
    Marker *prev,
    Marker *next,
    bool main,
    bool polarConnection)
{
	if (prev && next && main && (!first || polarConnection)) {
		next->prevMainMarker =
		    RelativeMarkerIndex{prev->idx, prev - next};
		next->polarConnection = polarConnection && first;
		return true;
	}

	return false;
}

Conv::JSONObj &&Marker::appendToJSON(Conv::JSONObj &&jsonObj) const
{
	return std::move(jsonObj).merge(cellInfo->json);
}

double Marker::getValueForChannel(const Channels &channels,
    ChannelId type,
    const Data::DataCube &data,
    ChannelStats &stats,
    const Data::MultiIndex &index,
    const Data::MarkerId *mid) const
{
	const auto &channel = channels.at(type);

	if (channel.isEmpty()) {
		static constexpr auto defVals =
		    Refl::EnumArray<ChannelId, double>::make(
		        {{ChannelId::color, 0.0},
		            {ChannelId::lightness, 0.5},
		            {ChannelId::size, 0.0},
		            {ChannelId::label, 0.0},
		            {ChannelId::x, 1.0},
		            {ChannelId::y, 1.0},
		            {ChannelId::noop, 0.0}});

		return defVals[type];
	}

	double value{};

	if (channel.isDimension()) {
		std::optional<Data::MarkerId> nid;
		if (!mid)
			nid.emplace(
			    data.getId(channel.dimensionsWithLevel(), index));

		const auto &id = mid ? *mid : *nid;
		if (channel.stackable)
			value = 1.0;
		else
			value = static_cast<double>(id.itemId);

		if (enabled) stats.track(type, id);
	}
	else {
		if (const auto &measure = *channel.measure();
		    channel.stackable)
			value = data.aggregateAt(index, type, measure);
		else
			value = data.valueAt(index, measure);

		if (enabled) stats.track(type, value);
	}
	return value;
}

Geom::Rect Marker::toRectangle() const
{
	return {position - size, {size}};
}

void Marker::fromRectangle(const Geom::Rect &rect)
{
	position = rect.pos + rect.size;
	size = rect.size;
}

Math::Range<> Marker::getSizeBy(AxisId axisId) const
{
	return isHorizontal(+axisId) ? toRectangle().hSize()
	                             : toRectangle().vSize();
}

void Marker::setSizeBy(AxisId axisId, const Math::Range<> range)
{
	auto rect = toRectangle();
	if (isHorizontal(+axisId))
		rect.setHSize(range);
	else
		rect.setVSize(range);
	fromRectangle(rect);
}

bool Marker::Label::operator==(const Label &other) const
{
	return value == other.value && indexStr == other.indexStr;
}
}
