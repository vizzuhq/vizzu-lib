#include "channel.h"

#include <algorithm>
#include <cmath>
#include <set>

namespace Vizzu::Gen
{

bool isAxis(ChannelId type)
{
	return type == ChannelId::x || type == ChannelId::y;
}

Channel Channel::makeChannel(Type id)
{
	switch (id) {
	case ChannelId::color: return {ChannelId::color, 0, false};
	case ChannelId::label: return {ChannelId::label, 0, false};
	case ChannelId::lightness:
		return {ChannelId::lightness, 0.5, false};
	case ChannelId::size: return {ChannelId::size, 0, true};
	case ChannelId::x: return {ChannelId::x, 1, true};
	case ChannelId::y: return {ChannelId::y, 1, true};
	case ChannelId::noop: return {ChannelId::noop, 0, false};
	default:;
	};
	throw std::logic_error("internal error: invalid channel id");
}

std::pair<bool, Channel::OptionalIndex> Channel::addSeries(
    const Data::SeriesIndex &index)
{
	if (index.isDimension()) {
		return {dimensionIds.push_back(index), std::nullopt};
	}
	if (!measureId) {
		measureId = index;
		return {true, std::nullopt};
	}
	if (*measureId != index) {
		auto replaced = *measureId;
		measureId = index;
		return {true, replaced};
	}
	return {false, std::nullopt};
}

void Channel::removeSeries(const Data::SeriesIndex &index)
{
	if (index.isDimension())
		dimensionIds.remove(index);
	else if (measureId)
		measureId = std::nullopt;
}

bool Channel::isSeriesUsed(const Data::SeriesIndex &index) const
{
	return (measureId && *measureId == index)
	    || dimensionIds.contains(index);
}

void Channel::reset()
{
	measureId = std::nullopt;
	dimensionIds.clear();
	title = Base::AutoParam<std::string, true>{};
	axisLine = Base::AutoBool();
	axisLabels = Base::AutoBool();
	ticks = Base::AutoBool();
	interlacing = Base::AutoBool();
	guides = Base::AutoBool();
	markerGuides = Base::AutoBool();
	labelLevel = 0;
}

bool Channel::isEmpty() const
{
	return (!measureId && dimensionIds.empty());
}

bool Channel::isDimension() const { return !measureId; }

bool Channel::isMeasure() const { return !isEmpty() && measureId; }

size_t Channel::dimensionCount() const { return dimensionIds.size(); }

void Channel::collectDimesions(
    Data::DataCubeOptions::IndexSet &dimensions) const
{
	for (const auto &dimension : dimensionIds)
		dimensions.insert(dimension);
}

bool Channel::operator==(const Channel &other) const
{
	return type == other.type && measureId == other.measureId
	    && dimensionIds == other.dimensionIds
	    && (defaultValue == other.defaultValue
	        || (std::isnan(defaultValue)
	            && std::isnan(other.defaultValue)))
	    && stackable == other.stackable && range == other.range
	    && labelLevel == other.labelLevel && title == other.title
	    && axisLine == other.axisLine
	    && axisLabels == other.axisLabels && ticks == other.ticks
	    && interlacing == other.interlacing && guides == other.guides
	    && markerGuides == other.markerGuides;
}

std::string Channel::measureName(const Data::DataCube &cube) const
{
	return measureId ? cube.getName(*measureId) : std::string{};
}

std::string Channel::labelDimensionName() const
{
	auto &&ser = labelSeries();
	return ser ? ser->toString() : "";
}

const Channel::DimensionIndices &Channel::dimensions() const
{
	return dimensionIds;
}

Channel::OptionalIndex Channel::labelSeries() const
{
	if (isDimension()) {
		if (labelLevel < dimensionIds.size())
			return *std::next(dimensionIds.begin(),
			    static_cast<std::intptr_t>(labelLevel));
		return std::nullopt;
	}
	return measureId;
}

}