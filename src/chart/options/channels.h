#ifndef CHANNELS_H
#define CHANNELS_H

#include <array>
#include <functional>
#include <vector>

#include "dataframe/old/types.h"

#include "channel.h"

namespace Vizzu::Gen
{

struct Channels : Refl::EnumArray<ChannelId, Channel>
{
	using IndexSet = std::set<Data::SeriesIndex>;

	[[nodiscard]] bool anyAxisSet() const;
	[[nodiscard]] bool isEmpty() const;

	[[nodiscard]] IndexSet getDimensions() const;
	[[nodiscard]] IndexSet getMeasures() const;
	[[nodiscard]] IndexSet getDimensions(
	    const std::span<const ChannelId> &channelTypes) const;

	using EnumArray::at;

	template <ChannelIdLike T>
	[[nodiscard]] const Channel &at(const T &id) const
	{
		return at(-id);
	}

	template <ChannelIdLike T> [[nodiscard]] Channel &at(const T &id)
	{
		return at(-id);
	}

	void removeSeries(const Data::SeriesIndex &index);

	[[nodiscard]] bool isSeriesUsed(
	    const Data::SeriesIndex &index) const;

	void reset();

	bool operator==(const Channels &other) const = default;

	[[nodiscard]] Channels shadow() const;
};

}

#endif
