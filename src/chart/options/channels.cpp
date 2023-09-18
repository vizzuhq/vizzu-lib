#include "channels.h"

Vizzu::Gen::Channels::Channels() :
    channels(
        []<std::size_t... Ix>(
            std::index_sequence<Ix...>) -> decltype(channels)
        {
	        return {
	            Channel::makeChannel(static_cast<ChannelId>(Ix))...};
        }(std::make_index_sequence<
            std::tuple_size_v<decltype(channels)::base_array>>{}))
{}

bool Vizzu::Gen::Channels::anyAxisSet() const
{
	return !channels[ChannelId::x].isEmpty()
	    || !channels[ChannelId::y].isEmpty();
}

bool Vizzu::Gen::Channels::oneAxisSet() const
{
	return anyAxisSet() && !bothAxisSet();
}

bool Vizzu::Gen::Channels::bothAxisSet() const
{
	return !channels[ChannelId::x].isEmpty()
	    && !channels[ChannelId::y].isEmpty();
}

bool Vizzu::Gen::Channels::isEmpty() const
{
	return std::ranges::all_of(channels,
	    [](const auto &channel)
	    {
		    return channel.isEmpty();
	    });
}

Vizzu::Data::DataCubeOptions::IndexSet
Vizzu::Gen::Channels::getDimensions() const
{
	Data::DataCubeOptions::IndexSet dimensions;

	for (const auto &channel : channels)
		channel.collectDimesions(dimensions);

	return dimensions;
}

Vizzu::Data::DataCubeOptions::IndexSet
Vizzu::Gen::Channels::getSeries() const
{
	Data::DataCubeOptions::IndexSet series;

	for (const auto &channel : channels)
		if (channel.measureId) {
			const auto &index = *channel.measureId;
			series.insert(index);
		}

	return series;
}

Vizzu::Data::DataCubeOptions::IndexSet
Vizzu::Gen::Channels::getDimensions(
    const std::vector<ChannelId> &channelTypes) const
{
	Data::DataCubeOptions::IndexSet dimensions;
	for (auto channelType : channelTypes)
		channels[static_cast<ChannelId>(channelType)]
		    .collectDimesions(dimensions);
	return dimensions;
}

Vizzu::Data::DataCubeOptions::IndexSet
Vizzu::Gen::Channels::getRealSeries(
    const std::vector<ChannelId> &channelTypes) const
{
	Data::DataCubeOptions::IndexSet series;
	for (auto channelType : channelTypes)
		channels[static_cast<ChannelId>(channelType)]
		    .collectRealSeries(series);
	return series;
}

Vizzu::Data::DataCubeOptions
Vizzu::Gen::Channels::getDataCubeOptions() const
{
	return {getDimensions(), getSeries()};
}

std::pair<bool, Vizzu::Gen::Channel::OptionalIndex>
Vizzu::Gen::Channels::addSeries(const ChannelId &id,
    const Data::SeriesIndex &index,
    std::optional<size_t> pos)
{
	return channels[id].addSeries(index, pos);
}

bool Vizzu::Gen::Channels::removeSeries(const ChannelId &id,
    const Data::SeriesIndex &index)
{
	return channels[id].removeSeries(index);
}

bool Vizzu::Gen::Channels::removeSeries(
    const Data::SeriesIndex &index)
{
	bool res = false;
	for (auto &channel : channels) res |= channel.removeSeries(index);
	return res;
}

bool Vizzu::Gen::Channels::clearSeries(const ChannelId &id)
{
	auto &channel = channels[id];
	if (channel.isEmpty()) return false;
	channels[id].reset();
	return true;
}

bool Vizzu::Gen::Channels::isSeriesUsed(
    const Data::SeriesIndex &index) const
{
	return std::ranges::any_of(channels,
	    [&](const auto &channel)
	    {
		    return channel.isSeriesUsed(index);
	    });
}

bool Vizzu::Gen::Channels::isSeriesUsed(
    const std::vector<ChannelId> &channelTypes,
    const Data::SeriesIndex &index) const
{
	return std::ranges::any_of(channelTypes,
	    [this, &index](auto channelType)
	    {
		    return channels[static_cast<ChannelId>(channelType)]
		        .isSeriesUsed(index);
	    });
}

size_t Vizzu::Gen::Channels::count(
    const Data::SeriesIndex &index) const
{
	size_t cnt = 0;
	for (const auto &channel : channels)
		if (channel.isSeriesUsed(index)) cnt++;
	return cnt;
}

std::list<Vizzu::Gen::ChannelId> Vizzu::Gen::Channels::find(
    const Data::SeriesIndex &index) const
{
	std::list<ChannelId> res;
	for (auto type = 0U; type < std::size(channels); type++) {
		if (channels[static_cast<ChannelId>(type)].isSeriesUsed(
		        index))
			res.push_back(static_cast<ChannelId>(type));
	}
	return res;
}

const Vizzu::Gen::Channel &Vizzu::Gen::Channels::at(
    const ChannelId &id) const
{
	return channels.at(id);
}

Vizzu::Gen::Channel &Vizzu::Gen::Channels::at(const ChannelId &id)
{
	return channels.at(id);
}

Vizzu::Gen::ChannelId Vizzu::Gen::Channels::getEmptyAxisId() const
{
	if (at(ChannelId::x).isEmpty()) return ChannelId::x;
	if (at(ChannelId::y).isEmpty()) return ChannelId::y;
	return static_cast<ChannelId>(std::size(channels));
}

void Vizzu::Gen::Channels::reset()
{
	for (auto &channel : channels) channel.reset();
}

bool Vizzu::Gen::Channels::operator==(const Channels &other) const
{
	return channels == other.channels;
}

void Vizzu::Gen::Channels::visitAll(
    const std::function<void(ChannelId, const Channel &)> &visitor)
    const
{
	for (auto type = 0U; type < std::size(channels); type++) {
		const auto &channel = channels[static_cast<ChannelId>(type)];
		visitor(static_cast<ChannelId>(type), std::ref(channel));
	}
}

Vizzu::Gen::Channels Vizzu::Gen::Channels::shadow() const
{
	Channels shadow = *this;

	auto attrs = getDimensions({ChannelId::color,
	    ChannelId::lightness,
	    ChannelId::label,
	    ChannelId::noop});

	shadow.channels[ChannelId::color].reset();
	shadow.channels[ChannelId::lightness].reset();
	shadow.channels[ChannelId::label].reset();
	shadow.channels[ChannelId::noop].reset();

	for (const auto &attr : attrs)
		shadow.channels[ChannelId::noop].addSeries(attr);

	return shadow;
}
