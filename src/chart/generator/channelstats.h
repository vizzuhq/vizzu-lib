#ifndef CHANNELSTATS_H
#define CHANNELSTATS_H

#include <vector>

#include "base/math/range.h"
#include "chart/options/channels.h"
#include "data/datacube/datacube.h"

#include "marker.h"

namespace Vizzu::Gen
{

class ChannelStats
{
public:
	bool isDimension;
	Math::Range<double> range;
	std::vector<Data::DataCube::Id::SubSliceIndex> usedIndices;

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
