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

Channel Channel::makeChannel(ChannelId id)
{
	static constexpr auto defVals =
	    Refl::EnumArray<ChannelId, double>::make(
	        {{ChannelId::color, 0.0},
	            {ChannelId::lightness, 0.5},
	            {ChannelId::size, 0.0},
	            {ChannelId::label, 0.0},
	            {ChannelId::x, 1.0},
	            {ChannelId::y, 1.0},
	            {ChannelId::noop, 0.0}});

	static constexpr auto defStackable =
	    Refl::EnumArray<ChannelId, bool>::make(
	        {{ChannelId::color, false},
	            {ChannelId::lightness, false},
	            {ChannelId::size, true},
	            {ChannelId::label, false},
	            {ChannelId::x, true},
	            {ChannelId::y, true},
	            {ChannelId::noop, false}});
	return {{defStackable[id]}, defVals[id]};
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
	title = Base::AutoParam<std::string, true>{};
	axis = Base::AutoBool();
	labels = Base::AutoBool();
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
	return measureId == other.measureId
	    && dimensionIds == other.dimensionIds
	    && std::is_eq(
	        std::weak_order(defaultValue, other.defaultValue))
	    && stackable == other.stackable && range == other.range
	    && labelLevel == other.labelLevel && title == other.title
	    && axis == other.axis && labels == other.labels
	    && ticks == other.ticks && interlacing == other.interlacing
	    && guides == other.guides
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