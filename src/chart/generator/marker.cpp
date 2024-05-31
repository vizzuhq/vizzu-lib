#include "marker.h"

#include "dataframe/old/datatable.h"

#include "axis.h"

namespace Vizzu::Gen
{

Marker::Marker(const Options &options,
    const Data::DataCube &data,
    Axises &stats,
    const Data::MultiIndex &index,
    MarkerIndex idx,
    bool needMarkerInfo) :
    enabled(data.empty() || !index.isEmpty()),
    cellInfo(enabled || needMarkerInfo
                 ? data.cellInfo(index, idx, needMarkerInfo)
                 : nullptr),
    sizeId(data.getId(options.getChannels()
                          .at(ChannelId::size)
                          .dimensionsWithLevel(),
        index)),
    idx(idx)
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

	auto &&mainAxisDims = options.mainAxis().dimensionsWithLevel();

	Data::MarkerId *subAxisId{};
	if (auto &&subAxis = options.subAxis().dimensionsWithLevel();
	    options.geometry == ShapeType::area) {
		Data::SeriesList subIds(subAxis.first);
		if (subIds.split_by(mainAxisDims.first).empty())
			subAxisId = &subId;
		subId = data.getId({subIds, subAxis.second}, index);
	}
	else {
		subId = data.getId(subAxis, index);
		subAxisId = &subId;
	}
	mainId = data.getId(mainAxisDims, index);

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
	    channels.at(ChannelId::y).isDimension()
	    && channels.at(ChannelId::y).hasDimension()
	    && options.geometry == ShapeType::rectangle
	    && options.align != Base::Align::Type::stretch;

	spacing.x =
	    (horizontal || (lineOrCircle && !polar) || yChannelRectDim)
	            && options.getChannels().anyAxisSet()
	            && channels.at(ChannelId::x).isDimension()
	        ? 1
	        : 0;

	position.y = size.y = getValueForChannel(channels,
	    ChannelId::y,
	    data,
	    stats,
	    index,
	    !horizontal ? &mainId : subAxisId);

	auto xChannelRectDim =
	    channels.at(ChannelId::x).isDimension()
	    && channels.at(ChannelId::x).hasDimension()
	    && options.geometry == ShapeType::rectangle
	    && options.align != Base::Align::Type::stretch;

	spacing.y = (!horizontal || lineOrCircle || xChannelRectDim)
	                 && options.getChannels().anyAxisSet()
	                 && channels.at(ChannelId::y).isDimension()
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

void Marker::setNextMarker(bool first,
    Marker &marker,
    bool horizontal,
    bool main)
{
	if (main) marker.prevMainMarkerIdx = idx;

	if (!first) {
		double Geom::Point::*const coord =
		    horizontal ? &Geom::Point::x : &Geom::Point::y;
		marker.position.*coord += position.*coord;
	}
	else if (main && this != &marker)
		marker.polarConnection = true;
}

void Marker::resetSize(bool horizontal)
{
	double Geom::Point::*const coord =
	    horizontal ? &Geom::Point::x : &Geom::Point::y;
	size.*coord = 0;
	position.*coord = 0;
}

void Marker::setIdOffset(size_t offset)
{
	if (prevMainMarkerIdx.hasOneValue())
		prevMainMarkerIdx->value += offset;
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
    Axises &stats,
    const Data::MultiIndex &index,
    const Data::MarkerId *mid) const
{
	const auto &channel = channels.at(type);

	if (channel.isEmpty()) return channel.defaultValue;

	double value{};

	auto &stat = stats.at(type);

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

		if (enabled) stat.dimension.track(id);
	}
	else {
		if (const auto &measure = *channel.measureId;
		    channel.stackable)
			value = data.aggregateAt(index, type, measure);
		else
			value = data.valueAt(index, measure);

		if (enabled) stat.measure.track(value);
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
