#include "channelstats.h"

#include "dataframe/old/datatable.h"

namespace Vizzu::Gen
{

ChannelStats::ChannelStats(const Channel &channel,
    const Data::DataCube &cube)
{
	if (channel.isDimension())
		stat.emplace<1>(
		    cube.combinedSizeOf(channel.dimensions()).second);
}

void ChannelStats::track(double value) { range().include(value); }

void ChannelStats::track(const Marker::Id &id)
{
	indices()[id.itemId] = id.label;
}

ChannelsStats::ChannelsStats(const Channels &paramchannels,
    const Data::DataCube &cube) :
    channels(
        [&]<std::size_t... Ix>(
            std::index_sequence<Ix...>) -> decltype(channels)
        {
	        return {ChannelStats(
	            paramchannels.at(static_cast<ChannelId>(Ix)),
	            cube)...};
        }(std::make_index_sequence<
            std::tuple_size_v<decltype(channels)::base_array>>{}))
{}

}