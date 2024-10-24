#ifndef CHANNELS_H
#define CHANNELS_H

#include <array>
#include <functional>
#include <vector>

#include "dataframe/old/types.h"

#include "channel.h"

namespace Vizzu::Gen
{

class Channels
{
public:
	using IndexSet = std::set<Data::SeriesIndex>;
	struct Pos
	{
		ChannelId channelId;
		int position;
	};

	[[nodiscard]] bool anyAxisSet() const;
	[[nodiscard]] bool isEmpty() const;

	[[nodiscard]] IndexSet getDimensions() const;
	[[nodiscard]] IndexSet getMeasures() const;
	[[nodiscard]] IndexSet getDimensions(
	    const std::span<const ChannelId> &channelTypes) const;

	[[nodiscard]] const Channel &at(const ChannelId &id) const;
	[[nodiscard]] Channel &at(const ChannelId &id);

	template <ChannelIdLike T>
	[[nodiscard]] const Channel &at(const T &id) const
	{
		return at(asChannel(id));
	}

	template <ChannelIdLike T> [[nodiscard]] Channel &at(const T &id)
	{
		return at(asChannel(id));
	}

	void removeSeries(const Data::SeriesIndex &index);

	[[nodiscard]] bool isSeriesUsed(
	    const Data::SeriesIndex &index) const;

	void reset();

	bool operator==(const Channels &other) const;

	[[nodiscard]] Channels shadow() const;

	[[nodiscard]] auto &getChannels() { return channels; }

private:
	Refl::EnumArray<ChannelId, Channel> channels =
	    []<std::size_t... Ix>(std::index_sequence<Ix...>)
	{
		return decltype(channels){
		    Channel::makeChannel(static_cast<ChannelId>(Ix))...};
	}(std::make_index_sequence<
	    std::tuple_size_v<decltype(channels)::base_array>>{});
};

}

#endif
