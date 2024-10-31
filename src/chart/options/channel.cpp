#include "channel.h"

#include <cmath>
#include <compare>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <optional>
#include <string>
#include <utility>

#include "base/refl/auto_enum.h"
#include "chart/options/autoparam.h"
#include "dataframe/old/datatable.h"
#include "dataframe/old/types.h"

namespace Vizzu::Gen
{

Channel Channel::makeChannel(ChannelId id)
{
	static constexpr auto defStackable =
	    Refl::EnumArray<ChannelId, bool>::make(
	        {{ChannelId::color, false},
	            {ChannelId::lightness, false},
	            {ChannelId::size, true},
	            {ChannelId::label, false},
	            {ChannelId::x, true},
	            {ChannelId::y, true},
	            {ChannelId::noop, false}});
	return {{defStackable[id]}};
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

void Channel::collectDimensions(IndexSet &dimensions) const
{
	for (const auto &dimension : dimensionIds)
		dimensions.insert(dimension);
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