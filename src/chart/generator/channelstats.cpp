#include "channelstats.h"

using namespace Vizzu;
using namespace Vizzu::Diag;

ChannelStats::ChannelStats(const Channel &channel, const Data::DataCube &cube)
{
	sum = 0.0;
	discrete = channel.isPseudoDiscrete();
	if (discrete)
		usedIndices = std::vector<Data::MultiDim::SubSliceIndex>(
		    cube.combinedSizeOf(channel.discretesIds()),
		    Data::MultiDim::SubSliceIndex());
}

void ChannelStats::track(double value)
{
	if (discrete)
		throw std::logic_error(
		    "internal error: invalid discrete channel tracking");
	else
		range.include(value);
}

void ChannelStats::trackSingle(double value)
{
	if (discrete)
		throw std::logic_error(
		    "internal error: invalid discrete channel tracking");
	else
		sum += value;
}

void ChannelStats::track(const Marker::Id &id)
{
	if (discrete)
		usedIndices[id.itemId] = id.itemSliceIndex;
	else
		throw std::logic_error(
		    "internal error: invalid continous channel tracking");
}

ChannelsStats::ChannelsStats(const Channels &channels,
    const Data::DataCube &cube)
{
	for (auto channelId = 0u;
	     channelId < ChannelId::EnumInfo::count();
	     channelId++) {
		const auto &channel = channels.at(ChannelId(channelId));

		this->channels[ChannelId(channelId)] = ChannelStats(channel, cube);
	}
}
