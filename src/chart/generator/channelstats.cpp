#include "channelstats.h"

using namespace Vizzu;
using namespace Vizzu::Gen;

ChannelStats::ChannelStats(const Channel &channel,
    const Data::DataCube &cube)
{
	sum = 0.0;
	isDimension = channel.isDimension();
	if (isDimension)
		usedIndices = std::vector<Data::MultiDim::SubSliceIndex>(
		    cube.combinedSizeOf(channel.dimensionIds),
		    Data::MultiDim::SubSliceIndex());
}

void ChannelStats::track(double value)
{
	if (isDimension)
		throw std::logic_error(
		    "internal error: invalid dimension channel tracking");
	else
		range.include(value);
}

void ChannelStats::trackSingle(double value)
{
	if (isDimension)
		throw std::logic_error(
		    "internal error: invalid dimension channel tracking");
	else
		sum += value;
}

void ChannelStats::track(const Marker::Id &id)
{
	if (isDimension)
		usedIndices[id.itemId] = id.itemSliceIndex;
	else
		throw std::logic_error(
		    "internal error: invalid measure channel tracking");
}

ChannelsStats::ChannelsStats(const Channels &channels,
    const Data::DataCube &cube)
{
	for (auto channelId = 0U; channelId < std::size(this->channels);
	     channelId++) {
		const auto &channel = channels.at(ChannelId(channelId));

		this->channels[ChannelId(channelId)] =
		    ChannelStats(channel, cube);
	}
}
