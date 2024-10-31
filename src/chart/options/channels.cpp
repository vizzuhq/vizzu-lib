#include "channels.h"

#include <algorithm>
#include <span>

#include "dataframe/old/types.h"

#include "channel.h"

namespace Vizzu::Gen
{

bool Channels::anyAxisSet() const
{
	return !at(AxisId::x).isEmpty() || !at(AxisId::y).isEmpty();
}

bool Channels::isEmpty() const
{
	return std::ranges::all_of(*this,
	    [](const auto &channel)
	    {
		    return channel.isEmpty();
	    });
}

Channels::IndexSet Channels::getDimensions() const
{
	IndexSet dimensions;

	for (const auto &channel : *this)
		channel.collectDimensions(dimensions);

	return dimensions;
}

Channels::IndexSet Channels::getMeasures() const
{
	IndexSet series;

	for (const auto &channel : *this)
		if (auto &&mid = channel.measureId) series.insert(*mid);

	return series;
}

Channels::IndexSet Channels::getDimensions(
    const std::span<const ChannelId> &channelTypes) const
{
	IndexSet dimensions;

	for (auto &&channelType : channelTypes)
		at(channelType).collectDimensions(dimensions);

	return dimensions;
}

void Channels::removeSeries(const Data::SeriesIndex &index)
{
	for (auto &channel : *this) channel.removeSeries(index);
}

bool Channels::isSeriesUsed(const Data::SeriesIndex &index) const
{
	return std::ranges::any_of(*this,
	    [&](const auto &channel)
	    {
		    return channel.isSeriesUsed(index);
	    });
}

void Channels::reset()
{
	for (auto &channel : *this) channel.reset();
}

Channels Channels::shadow() const
{
	Channels shadow = *this;

	shadow[ChannelId::color].reset();
	shadow[ChannelId::lightness].reset();
	shadow[ChannelId::label].reset();
	shadow[ChannelId::noop].reset();

	for (auto &&attr : getDimensions({{ChannelId::color,
	         ChannelId::lightness,
	         ChannelId::label,
	         ChannelId::noop}}))
		shadow[ChannelId::noop].addSeries(attr);

	return shadow;
}

}