#include "channel.h"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>

#include "chart/options/autoparam.h"
#include "dataframe/old/datatable.h"
#include "dataframe/old/types.h"

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

void Channel::addSeries(const Data::SeriesIndex &index)
{
	if (index.isDimension())
		dimensionIds.push_back(index);
	else
		measureId = index;
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
	title = Base::AutoParam<Text::immutable_string, true>{};
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
	return !measureId && dimensionIds.empty();
}

bool Channel::isDimension() const { return !measureId; }

bool Channel::hasDimension() const { return !dimensionIds.empty(); }

bool Channel::isMeasure() const { return measureId.has_value(); }

void Channel::collectDimesions(IndexSet &dimensions) const
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

const Channel::DimensionIndices &Channel::dimensions() const
{
	return dimensionIds;
}

std::pair<const Channel::DimensionIndices &, const std::size_t &>
Channel::dimensionsWithLevel() const
{
	return {dimensionIds, labelLevel};
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