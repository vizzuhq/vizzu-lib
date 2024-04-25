#include "channels.h"

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

Data::DataCubeOptions::IndexSet Channels::getDimensions() const
{
	Data::DataCubeOptions::IndexSet dimensions;

	for (const auto &channel : channels)
		channel.collectDimesions(dimensions);

	return dimensions;
}

Data::DataCubeOptions::IndexSet Channels::getMeasures() const
{
	Data::DataCubeOptions::IndexSet series;

	for (const auto &channel : channels)
		if (channel.measureId) {
			const auto &index = *channel.measureId;
			series.insert(index);
		}

	return series;
}

Data::DataCubeOptions::IndexSet Channels::getDimensions(
    const std::vector<ChannelId> &channelTypes) const
{
	Data::DataCubeOptions::IndexSet dimensions;
	for (auto channelType : channelTypes)
		channels[static_cast<ChannelId>(channelType)]
		    .collectDimesions(dimensions);
	return dimensions;
}

Data::DataCubeOptions Channels::getDataCubeOptions() const
{
	return {getDimensions(), getMeasures()};
}

std::pair<bool, Channel::OptionalIndex> Channels::addSeries(
    const ChannelId &id,
    const Data::SeriesIndex &index)
{
	return channels[id].addSeries(index);
}

void Channels::removeSeries(const ChannelId &id,
    const Data::SeriesIndex &index)
{
	channels[id].removeSeries(index);
}

void Channels::removeSeries(const Data::SeriesIndex &index)
{
	for (auto &channel : channels) channel.removeSeries(index);
}

bool Channels::clearSeries(const ChannelId &id)
{
	auto &channel = channels[id];
	if (channel.isEmpty()) return false;
	channels[id].reset();
	return true;
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

	auto attrs = getDimensions({ChannelId::color,
	    ChannelId::lightness,
	    ChannelId::label,
	    ChannelId::noop});

	shadow.channels[ChannelId::color].reset();
	shadow.channels[ChannelId::lightness].reset();
	shadow.channels[ChannelId::label].reset();
	shadow.channels[ChannelId::noop].reset();

	for (const auto &attr : attrs)
		shadow.channels[ChannelId::noop].addSeries(attr);

	return shadow;
}

}