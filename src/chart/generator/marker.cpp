#include "marker.h"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <utility>

#include "base/conv/auto_json.h"
#include "base/geom/point.h"
#include "base/geom/rect.h"
#include "base/math/range.h"
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
    enabled(true),
    cellInfo(enabled || needMarkerInfo
                 ? data.cellInfo(index, needMarkerInfo)
                 : nullptr),
    sizeId(data.getId(options.getChannels()
                          .at(ChannelId::size)
                          .dimensionsWithLevel(),
        index)),
    idx(index.marker_id)
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
		    MarkerIndexPosition{prev->idx, prev->pos};
		next->polarConnection = polarConnection && first;
		return true;
	}
	if (next && main) {
		next->prevMainMarker =
		    MarkerIndexPosition{next->idx, next->pos};
	}

	return false;
}

void Marker::setIdOffset(size_t offset)
{
	if (prevMainMarker.hasOneValue())
		prevMainMarker->value.pos += offset;
}

Conv::JSONObj &&Marker::appendToJSON(Conv::JSONObj &&jsonObj) const
{
	if (cellInfo) return std::move(jsonObj).merge(cellInfo->json);
	jsonObj.nested("categories");
	jsonObj.nested("values");
	return std::move(jsonObj)("index", idx);
}

double Marker::getValueForChannel(const Channels &channels,
    ChannelId type,
    const Data::DataCube &data,
    ChannelStats &stats,
    const Data::MultiIndex &index,
    const Data::MarkerId *mid) const
{
	const auto &channel = channels.at(type);

	if (channel.isEmpty()) return channel.defaultValue;

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
		if (const auto &measure = *channel.measureId;
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

Math::Range<double> Marker::getSizeBy(bool horizontal) const
{
	return horizontal ? toRectangle().hSize() : toRectangle().vSize();
}

void Marker::setSizeBy(bool horizontal,
    const Math::Range<double> range)
{
	auto rect = toRectangle();
	if (horizontal)
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
