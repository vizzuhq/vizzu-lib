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
	return std::ranges::all_of(genProps,
	    [](const auto &channel)
	    {
		    return channel.isEmpty();
	    });
}

Channels::IndexSet Channels::getDimensions() const
{
	IndexSet dimensions;

	for (const auto &channel : genProps)
		channel.collectDimensions(dimensions);

	return dimensions;
}

Channels::IndexSet Channels::getMeasures() const
{
	IndexSet series;

	for (const auto &channel : genProps)
		if (auto &&mid = channel.measure()) series.insert(*mid);

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
	for (auto &channel : genProps) channel.removeSeries(index);
}

bool Channels::isSeriesUsed(const Data::SeriesIndex &index) const
{
	return std::ranges::any_of(genProps,
	    [&](const auto &channel)
	    {
		    return channel.isSeriesUsed(index);
	    });
}

void Channels::reset()
{
	for (auto &channel : genProps) channel.reset();
}

Channels Channels::shadow() const
{
	Channels shadow{*this};

	shadow.genProps[ChannelId::color].reset();
	shadow.genProps[ChannelId::lightness].reset();
	shadow.genProps[ChannelId::label].reset();
	shadow.genProps[ChannelId::noop].reset();

	for (auto &&attr : getDimensions({{ChannelId::color,
	         ChannelId::lightness,
	         ChannelId::label,
	         ChannelId::noop}}))
		shadow.genProps[ChannelId::noop].addSeries(attr);

	return shadow;
}

}