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
	std::variant<Math::Range<double>,
	    std::vector<std::optional<Data::SliceIndex>>>
	    stat;

	ChannelStats(const Channel &channel, const Data::DataCube &cube);

	void track(double value);
	void track(const Marker::Id &id);

	Math::Range<double> &range()
	{
		thread_local Math::Range<double> dummy{};
		auto *range = std::get_if<Math::Range<double>>(&stat);
		return range ? *range : dummy;
	}

	[[nodiscard]] std::vector<std::optional<Data::SliceIndex>> &
	indices()
	{
		return *std::get_if<1>(&stat);
	}
};

class ChannelsStats
{
public:
	ChannelsStats(const Channels &paramchannels,
	    const Data::DataCube &cube);

	Refl::EnumArray<ChannelId, ChannelStats> channels;
};

}

#endif
