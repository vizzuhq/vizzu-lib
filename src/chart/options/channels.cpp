#include "channels.h"

#include <algorithm>
#include <span>

#include "dataframe/old/types.h"

#include "channel.h"

namespace Vizzu::Gen
{

bool Channels::anyAxisSet() const
{
	return !channels[ChannelId::x].isEmpty()
	    || !channels[ChannelId::y].isEmpty();
}

bool Channels::isEmpty() const
{
	return std::ranges::all_of(channels,
	    [](const auto &channel)
	    {
		    return channel.isEmpty();
	    });
}

Channels::IndexSet Channels::getDimensions() const
{
	IndexSet dimensions;

	for (const auto &channel : channels)
		channel.collectDimesions(dimensions);

	return dimensions;
}

Channels::IndexSet Channels::getMeasures() const
{
	IndexSet series;

	for (const auto &channel : channels)
		if (auto &&mid = channel.measureId) series.insert(*mid);

	return series;
}

Channels::IndexSet Channels::getDimensions(
    const std::span<const ChannelId> &channelTypes) const
{
	IndexSet dimensions;

	for (auto &&channelType : channelTypes)
		channels[channelType].collectDimesions(dimensions);

	return dimensions;
}

void Channels::removeSeries(const Data::SeriesIndex &index)
{
	for (auto &channel : channels) channel.removeSeries(index);
}

bool Channels::isSeriesUsed(const Data::SeriesIndex &index) const
{
	return std::ranges::any_of(channels,
	    [&](const auto &channel)
	    {
		    return channel.isSeriesUsed(index);
	    });
}

const Channel &Channels::at(const ChannelId &id) const
{
	return channels.at(id);
}

Channel &Channels::at(const ChannelId &id) { return channels.at(id); }

void Channels::reset()
{
	for (auto &channel : channels) channel.reset();
}

bool Channels::operator==(const Channels &other) const
{
	return channels == other.channels;
}

Channels Channels::shadow() const
{
	Channels shadow = *this;

	shadow.channels[ChannelId::color].reset();
	shadow.channels[ChannelId::lightness].reset();
	shadow.channels[ChannelId::label].reset();
	shadow.channels[ChannelId::noop].reset();

	for (auto &&attr : getDimensions({{ChannelId::color,
	         ChannelId::lightness,
	         ChannelId::label,
	         ChannelId::noop}}))
		shadow.channels[ChannelId::noop].addSeries(attr);

	return shadow;
}

}