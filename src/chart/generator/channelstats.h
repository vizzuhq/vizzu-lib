#ifndef CHANNELSTATS_H
#define CHANNELSTATS_H

#include <vector>

#include "base/math/range.h"
#include "chart/options/channels.h"
#include "data/datacube/datacube.h"

#include "marker.h"

namespace Vizzu
{
namespace Diag
{

class ChannelStats
{
public:
	bool discrete;
	Math::Range<double> range;
	double sum;
	std::vector<Data::MultiDim::SubSliceIndex> usedIndices;

	ChannelStats() : discrete(true) {}
	ChannelStats(const Channel &channel, const Data::DataCube &cube);

	void track(double value);
	void trackSingle(double value);
	void track(const Marker::Id &id);
};

class ChannelsStats
{
public:
	ChannelsStats() = default;
	ChannelsStats(const Channels &channels, const Data::DataCube &cube);

	std::array<ChannelStats, ChannelId::EnumInfo::count()> channels;
};

}
}

#endif
