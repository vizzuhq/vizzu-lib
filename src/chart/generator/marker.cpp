#include "marker.h"

#include "base/text/smartstring.h"
#include "chart/main/style.h"

#include "channelstats.h"

using namespace Vizzu;
using namespace Vizzu::Gen;
using namespace Geom;

Marker::Id::Id(const Data::DataCube &data,
    const Channel::DimensionIndices &dimensionIds,
    const Data::MultiDim::MultiIndex &index)
{
	seriesId = data.subSliceID(dimensionIds, index);
	itemSliceIndex = data.subSliceIndex(dimensionIds, index);
	itemId = data.getData().unfoldSubSliceIndex(itemSliceIndex);
}

Marker::Marker(const Options &options,
    const Styles::Chart &style,
    const Data::DataCube &data,
    const Data::DataTable &table,
    ChannelsStats &stats,
    const Data::MultiDim::MultiIndex &index,
    size_t idx) :
    index(index),
    idx(idx)
{
	cellInfo = data.cellInfo(index);

	enabled = data.subCellSize() == 0
	       || !data.getData().at(index).subCells[0].isEmpty();

	const auto &channels = options.getChannels();

	auto color =
	    getValueForChannel(channels, ChannelId::color, data, stats);

	auto lightness = getValueForChannel(channels,
	    ChannelId::lightness,
	    data,
	    stats);

	if (channels.at(ChannelId::color).isDimension()) {
		colorBuilder =
		    ColorBuilder(style.plot.marker.lightnessRange(),
		        *style.plot.marker.colorPalette,
		        static_cast<int>(color),
		        lightness);
	}
	else {
		colorBuilder =
		    ColorBuilder(style.plot.marker.lightnessRange(),
		        *style.plot.marker.colorGradient,
		        color,
		        lightness);
	}
	sizeFactor = getValueForChannel(channels,
	    ChannelId::size,
	    data,
	    stats,
	    options.subAxisOf(ChannelId::size));
	sizeId =
	    Id(data, channels.at(ChannelId::size).dimensionIds, index);

	mainId = Id(data, options.mainAxis().dimensionIds, index);

	auto stackInhibitingShape = options.geometry == ShapeType::area;
	if (stackInhibitingShape) {
		Data::SeriesList subIds(options.subAxis().dimensionIds);
		subIds.remove(options.mainAxis().dimensionIds);
		subId = Id(data, subIds, index);
		Data::SeriesList stackIds(options.subAxis().dimensionIds);
		stackIds.section(options.mainAxis().dimensionIds);
		stackId = Id(data, stackIds, index);
	}
	else {
		stackId = subId =
		    Id(data, options.subAxis().dimensionIds, index);
	}

	auto horizontal = options.isHorizontal();
	position.x = size.x = getValueForChannel(channels,
	    ChannelId::x,
	    data,
	    stats,
	    options.subAxisOf(ChannelId::x),
	    !horizontal && stackInhibitingShape);

	spacing.x =
	    (horizontal && options.getChannels().anyAxisSet()
	        && channels.at(ChannelId::x).isDimension())
	        ? 1
	        : 0;

	position.y = size.y = getValueForChannel(channels,
	    ChannelId::y,
	    data,
	    stats,
	    options.subAxisOf(ChannelId::y),
	    horizontal && stackInhibitingShape);

	spacing.y =
	    (!horizontal && options.getChannels().anyAxisSet()
	        && channels.at(ChannelId::y).isDimension())
	        ? 1
	        : 0;

	if (channels.at(ChannelId::label).isEmpty())
		label = ::Anim::Weighted<Label>(Label(), 0.0);
	else {
		auto value = getValueForChannel(channels,
		    ChannelId::label,
		    data,
		    stats);
		auto sliceIndex = data.subSliceIndex(
		    channels.at(ChannelId::label).dimensionIds,
		    index);
		if (channels.at(ChannelId::label).isDimension())
			label = Label(sliceIndex, data, table);
		else
			label = Label(value,
			    *channels.at(ChannelId::label).measureId,
			    sliceIndex,
			    data,
			    table);
	}
}

void Marker::setNextMarker(uint64_t itemId,
    Marker *marker,
    bool horizontal,
    bool main)
{
	if (marker) {
		(main ? nextMainMarkerIdx : nextSubMarkerIdx) = marker->idx;

		if (main) marker->prevMainMarkerIdx = idx;

		if (itemId != 0) {
			double Point::*const coord =
			    horizontal ? &Point::x : &Point::y;
			marker->position.*coord += position.*coord;
		}
	}
}

void Marker::resetSize(bool horizontal)
{
	double Point::*const coord = horizontal ? &Point::x : &Point::y;
	size.*coord = 0;
	position.*coord = 0;
}

void Marker::setIdOffset(size_t offset)
{
	if (prevMainMarkerIdx.hasOneValue())
		(*prevMainMarkerIdx).value += offset;
	if (nextMainMarkerIdx.hasOneValue())
		(*nextMainMarkerIdx).value += offset;
	if (nextSubMarkerIdx.hasOneValue())
		(*nextSubMarkerIdx).value += offset;
}

std::string Marker::toJson(const Data::DataTable &table) const
{
	auto categories = Text::SmartString::map(cellInfo.categories,
	    [&table](const auto &pair)
	    {
		    auto key =
		        Text::SmartString::escape(pair.first.toString(table));
		    auto colIndex = pair.first.getColIndex();
		    auto numValue = table.getInfo(colIndex.value())
		                        .categories()[pair.second];
		    auto value = Text::SmartString::escape(numValue);
		    return "\"" + key + "\":\"" + value + "\"";
	    });
	auto values = Text::SmartString::map(cellInfo.values,
	    [&table](const auto &pair)
	    {
		    auto key =
		        Text::SmartString::escape(pair.first.toString(table));
		    auto value = std::to_string(pair.second);
		    return "\"" + key + "\":" + value;
	    });
	return "{"
	       "\"categories\":{"
	     + Text::SmartString::join(categories)
	     + "},"
	       "\"values\":{"
	     + Text::SmartString::join(values)
	     + "},"
	       "\"id\":"
	     + std::to_string(idx) + "}";
}

double Marker::getValueForChannel(const Channels &channels,
    ChannelId type,
    const Data::DataCube &data,
    ChannelsStats &stats,
    const Channel *subChannel,
    bool inhibitStack) const
{
	const auto &channel = channels.at(type);

	if (channel.isEmpty()) return channel.defaultValue;

	Channel::DimensionIndices sumBy;

	if (subChannel) {
		if (inhibitStack) {
			for (auto id : subChannel->dimensionIds)
				if (channel.isSeriesUsed(id)) sumBy.pushBack(id);
		}
		else {
			sumBy = subChannel->dimensionIds;
			for (auto id : channel.dimensionIds) sumBy.remove(id);
		}
	}

	auto measure = channel.measureId;

	double value;
	auto id = Id(data, channel.dimensionIds, index);

	auto &stat = stats.channels[type];

	if (channel.isDimension()) {
		if (channel.stackable)
			value = 1.0;
		else
			value = static_cast<double>(id.itemId);

		if (enabled)
			stat.track(id);
	}
	else {
		auto singlevalue =
		    static_cast<double>(data.valueAt(index, *measure));

		if (channel.stackable)
			value = static_cast<double>(
			    data.aggregateAt(index, sumBy, *measure));
		else
			value = singlevalue;

		if (enabled) {
			if (measure) stat.trackSingle(singlevalue);
			stat.track(value);
		}
	}
	return value;
}

Rect Marker::toRectangle() const
{
	return {position - size, size};
}

void Marker::fromRectangle(const Rect &rect)
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

Marker::Label::Label(const Data::MultiDim::SubSliceIndex &index,
    const Data::DataCube &data,
    const Data::DataTable &table)
{
	indexStr = getIndexString(index, data, table);
}

Marker::Label::Label(double value,
    const Data::SeriesIndex &measure,
    const Data::MultiDim::SubSliceIndex &index,
    const Data::DataCube &data,
    const Data::DataTable &table) :
    value(value),
    measureId(measure.getColIndex())
{
	if (measureId) unit = table.getInfo(measureId.value()).getUnit();
	indexStr = getIndexString(index, data, table);
}

bool Marker::Label::operator==(const Marker::Label &other) const
{
	return measureId == other.measureId && value == other.value
	    && unit == other.unit && indexStr == other.indexStr;
}

std::string Marker::Label::getIndexString(
    const Data::MultiDim::SubSliceIndex &index,
    const Data::DataCube &data,
    const Data::DataTable &table)
{
	std::string res;

	for (const auto& [dimIx, ix] : index) {
		if (!res.empty()) res += ", ";
		auto colIndex =
		    data.getSeriesByDim(dimIx).getColIndex();
		auto value = table.getInfo(colIndex.value())
		                 .categories()[ix];
		res += value;
	}
	return res;
}
