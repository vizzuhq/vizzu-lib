#ifndef CHANNELS_H
#define CHANNELS_H

#include <array>
#include <functional>
#include <vector>

#include "base/type/uniquetype.h"
#include "data/datacube/datacubeoptions.h"

#include "channel.h"

namespace Vizzu
{
namespace Gen
{

class Channels
{
public:
	struct Pos
	{
		ChannelId channelId;
		int position;
	};

	Channels();

	bool anyAxisSet() const;
	bool oneAxisSet() const;
	bool bothAxisSet() const;
	bool isEmpty() const;

	Data::DataCubeOptions::IndexSet getDimensions() const;
	Data::DataCubeOptions::IndexSet getSeries() const;
	Data::DataCubeOptions::IndexSet getDimensions(
	    const std::vector<ChannelId> &channelTypes) const;
	Data::DataCubeOptions::IndexSet getRealSeries(
	    const std::vector<ChannelId> &channelTypes) const;
	Data::DataCubeOptions getDataCubeOptions() const;

	const Channel &at(const ChannelId &id) const;
	Channel &at(const ChannelId &id);
	ChannelId getEmptyAxisId() const;

	std::pair<bool, Channel::OptionalIndex> addSeries(const ChannelId &id,
	    const Data::SeriesIndex &index,
	    std::optional<size_t> pos = std::nullopt);
	bool removeSeries(const Data::SeriesIndex &index);
	bool removeSeries(const ChannelId &id,
	    const Data::SeriesIndex &index);
	bool clearSeries(const ChannelId &id);

	bool isSeriesUsed(const Data::SeriesIndex &index) const;
	bool isSeriesUsed(const std::vector<ChannelId> &channelTypes,
	    const Data::SeriesIndex &index) const;
	size_t count(const Data::SeriesIndex &index) const;
	std::list<ChannelId> find(const Data::SeriesIndex &index) const;
	std::list<Channels::Pos> findPos(
	    const Data::SeriesIndex &index) const;

	void reset();

	bool operator==(const Channels &other) const;

	void visitAll(
	    const std::function<void(ChannelId, const Channel &)> &visitor)
	    const;

	Channels shadow() const;

private:
	std::array<Channel, ChannelId::EnumInfo::count()> channels;
};

}
}

#endif
