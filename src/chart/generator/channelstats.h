#ifndef CHANNELSTATS_H
#define CHANNELSTATS_H

#include <vector>

#include "base/math/range.h"
#include "chart/options/channels.h"
#include "dataframe/old/types.h"

#include "marker.h"

namespace Vizzu::Gen
{

class ChannelStats
{
public:
	bool isDimension;
	Math::Range<double> range;
	std::vector<std::optional<Data::SliceIndex>> usedIndices;

	ChannelStats() : isDimension(true) {}
	ChannelStats(const Channel &channel, const Data::DataCube &cube);

	void track(double value);
	void track(const Marker::Id &id);
};

class ChannelsStats
{
public:
	ChannelsStats() = default;
	ChannelsStats(const Channels &channels,
	    const Data::DataCube &cube);

	Refl::EnumArray<ChannelId, ChannelStats> channels;
};

}

#endif
