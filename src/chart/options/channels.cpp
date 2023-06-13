#include "channels.h"

using namespace Vizzu;
using namespace Vizzu::Gen;

Channels::Channels()
{
	for (auto type = 0u; type < ChannelId::EnumInfo::count(); type++)
		channels[(ChannelId)type] =
		    Channel::makeChannel(ChannelId(type));

	reset();
}

bool Channels::anyAxisSet() const
{
	return !channels[ChannelId::x].isEmpty()
	    || !channels[ChannelId::y].isEmpty();
}

bool Channels::oneAxisSet() const
{
	return anyAxisSet() && !bothAxisSet();
}

bool Channels::bothAxisSet() const
{
	return !channels[ChannelId::x].isEmpty()
	    && !channels[ChannelId::y].isEmpty();
}

bool Channels::isEmpty() const
{
	for (const auto &channel : channels)
		if (!channel.isEmpty()) return false;
	return true;
}

Data::DataCubeOptions::IndexSet Channels::getDimensions() const
{
	Data::DataCubeOptions::IndexSet dimensions;

	for (const auto &channel : channels)
		channel.collectDimesions(dimensions);

	return dimensions;
}

Data::DataCubeOptions::IndexSet Channels::getSeries() const
{
	Data::DataCubeOptions::IndexSet series;

	for (const auto &channel : channels)
		if (channel.measureId()) {
			const auto &index = *channel.measureId();
			series.insert(index);
		}

	return series;
}

Data::DataCubeOptions::IndexSet Channels::getDimensions(
    const std::vector<ChannelId> &channelTypes) const
{
	Data::DataCubeOptions::IndexSet dimensions;
	for (auto channelType : channelTypes)
		channels[(ChannelId)channelType].collectDimesions(dimensions);
	return dimensions;
}

Data::DataCubeOptions::IndexSet Channels::getRealSeries(
    const std::vector<ChannelId> &channelTypes) const
{
	Data::DataCubeOptions::IndexSet series;
	for (auto channelType : channelTypes)
		channels[(ChannelId)channelType].collectRealSeries(series);
	return series;
}

Data::DataCubeOptions Channels::getDataCubeOptions() const
{
	return Data::DataCubeOptions(getDimensions(), getSeries());
}

std::pair<bool, Channel::OptionalIndex> Channels::addSeries(
    const ChannelId &id,
    const Data::SeriesIndex &index,
    std::optional<size_t> pos)
{
	return channels[id].addSeries(index, pos);
}

bool Channels::removeSeries(const ChannelId &id,
    const Data::SeriesIndex &index)
{
	return channels[id].removeSeries(index);
}

bool Channels::removeSeries(const Data::SeriesIndex &index)
{
	bool res = false;
	for (auto &channel : channels) res |= channel.removeSeries(index);
	return res;
}

bool Channels::clearSeries(const ChannelId &id)
{
	auto &channel = channels[id];
	if (channel.isEmpty()) return false;
	channels[id].reset();
	return true;
}

bool Channels::isSeriesUsed(const Data::SeriesIndex &index) const
{
	for (const auto &channel : channels)
		if (channel.isSeriesUsed(index)) return true;
	return false;
}

bool Channels::isSeriesUsed(const std::vector<ChannelId> &channelTypes,
    const Data::SeriesIndex &index) const
{
	for (auto channelType : channelTypes)
		if (channels[(ChannelId)channelType].isSeriesUsed(index))
			return true;
	return false;
}

size_t Channels::count(const Data::SeriesIndex &index) const
{
	size_t cnt = 0;
	for (const auto &channel : channels)
		if (channel.isSeriesUsed(index)) cnt++;
	return cnt;
}

std::list<ChannelId> Channels::find(const Data::SeriesIndex &index) const
{
	std::list<ChannelId> res;
	for (auto type = 0u; type < ChannelId::EnumInfo::count(); type++) {
		if (channels[(ChannelId)type].isSeriesUsed(index))
			res.push_back((ChannelId)type);
	}
	return res;
}

std::list<Channels::Pos> Channels::findPos(
    const Data::SeriesIndex &index) const
{
	std::list<Channels::Pos> res;
	for (auto type = 0u; type < ChannelId::EnumInfo::count(); type++) {
		auto pos = channels[(ChannelId)type].findPos(index);
		if (pos >= 0) res.push_back({(ChannelId)type, pos});
	}
	return res;
}

const Channel &Channels::at(const ChannelId &id) const
{
	return channels.at(id);
}

Channel &Channels::at(const ChannelId &id) { return channels.at(id); }

ChannelId Channels::getEmptyAxisId() const
{
	if (at(ChannelId::x).isEmpty()) return ChannelId::x;
	if (at(ChannelId::y).isEmpty()) return ChannelId::y;
	return ChannelId(ChannelId::EnumInfo::count());
}

void Channels::reset()
{
	for (auto &channel : channels) channel.reset();
}

bool Channels::operator==(const Channels &other) const
{
	return channels == other.channels;
}

void Channels::visitAll(
    const std::function<void(ChannelId, const Channel &)> &visitor) const
{
	for (auto type = 0u; type < ChannelId::EnumInfo::count(); type++) {
		const auto &channel = channels[(ChannelId)type];
		visitor((ChannelId)type, std::ref(channel));
	}
}

Channels Channels::shadow() const
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

	for (auto &attr : attrs)
		shadow.channels[ChannelId::noop].addSeries(attr);

	return shadow;
}
