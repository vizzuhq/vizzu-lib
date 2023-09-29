#include "optionssetter.h"

#include <cmath>
#include <utility>

#include "base/text/smartstring.h"
#include "data/table/datatable.h"

namespace Vizzu::Gen
{

OptionsSetter::OptionsSetter(Options &options) : options(options) {}

void OptionsSetter::setTable(const Data::DataTable *table)
{
	this->table = table;
}

OptionsSetter &OptionsSetter::addSeries(const ChannelId &channelId,
    const std::string &seriesName,
    std::optional<size_t> pos)
{
	if (table) {
		auto index = Data::SeriesIndex(seriesName, *table);
		addSeries(channelId, index, pos);
	}
	else
		throw std::logic_error("no table set for options");

	return *this;
}

OptionsSetter &OptionsSetter::deleteSeries(const ChannelId &channelId,
    const std::string &seriesName)
{
	if (table) {
		auto index = Data::SeriesIndex(seriesName, *table);
		deleteSeries(channelId, index);
	}
	else
		throw std::logic_error("no table set for options");

	return *this;
}

OptionsSetter &OptionsSetter::addSeries(const ChannelId &channelId,
    const Data::SeriesIndex &index,
    std::optional<size_t> pos)
{
	options.markersInfo.clear();
	options.getChannels().addSeries(channelId, index, pos);
	return *this;
}

OptionsSetter &OptionsSetter::deleteSeries(const ChannelId &channelId,
    const Data::SeriesIndex &index)
{
	options.markersInfo.clear();
	options.getChannels().removeSeries(channelId, index);
	return *this;
}

OptionsSetter &OptionsSetter::clearSeries(const ChannelId &channelId)
{
	options.markersInfo.clear();
	options.getChannels().clearSeries(channelId);
	return *this;
}

OptionsSetter &OptionsSetter::rotate(double ccwQuadrant)
{
	options.angle += ccwQuadrant * M_PI / 2;
	return *this;
}

OptionsSetter &OptionsSetter::setAngle(double ccwQuadrant)
{
	options.angle = ccwQuadrant * M_PI / 2;
	return *this;
}

OptionsSetter &OptionsSetter::setFilter(const Data::Filter &filter)
{
	options.dataFilter = filter;
	return *this;
}

OptionsSetter &OptionsSetter::setLabelLevel(
    const ChannelId &channelId,
    std::size_t level)
{
	options.getChannels().at(channelId).labelLevel = level;
	return *this;
}

OptionsSetter &OptionsSetter::setRangeMin(const ChannelId &channelId,
    const OptionalChannelExtrema &value)
{
	options.getChannels().at(channelId).range.min = value;
	return *this;
}

OptionsSetter &OptionsSetter::setRangeMax(const ChannelId &channelId,
    const OptionalChannelExtrema &value)
{
	options.getChannels().at(channelId).range.max = value;
	return *this;
}

OptionsSetter &OptionsSetter::setStackable(const ChannelId &channelId,
    bool value)
{
	options.getChannels().at(channelId).stackable = value;
	return *this;
}

OptionsSetter &OptionsSetter::setAxisTitle(const ChannelId &channelId,
    const std::string &title)
{
	options.getChannels().at(channelId).title = title;
	return *this;
}

OptionsSetter &OptionsSetter::setAxisLine(const ChannelId &channelId,
    Base::AutoBool enable)
{
	options.getChannels().at(channelId).axisLine = enable;
	return *this;
}

OptionsSetter &OptionsSetter::setAxisLabels(
    const ChannelId &channelId,
    Base::AutoBool enable)
{
	options.getChannels().at(channelId).axisLabels = enable;
	return *this;
}

OptionsSetter &OptionsSetter::setTicks(const ChannelId &channelId,
    Base::AutoBool enable)
{
	options.getChannels().at(channelId).ticks = enable;
	return *this;
}

OptionsSetter &OptionsSetter::setGuides(const ChannelId &channelId,
    Base::AutoBool enable)
{
	options.getChannels().at(channelId).guides = enable;
	return *this;
}

OptionsSetter &OptionsSetter::setMarkerGuides(
    const ChannelId &channelId,
    Base::AutoBool enable)
{
	options.getChannels().at(channelId).markerGuides = enable;
	return *this;
}

OptionsSetter &OptionsSetter::setInterlacing(
    const ChannelId &channelId,
    Base::AutoBool enable)
{
	options.getChannels().at(channelId).interlacing = enable;
	return *this;
}

OptionsSetter &OptionsSetter::setStep(const ChannelId &channelId,
    Base::AutoParam<double> step)
{
	options.getChannels().at(channelId).step = step;
	return *this;
}

OptionsSetter &OptionsSetter::addMarkerInfo(Options::MarkerId marker)
{
	if (!options.getMarkerInfoId(marker).has_value()) {
		auto miid = Options::generateMarkerInfoId();
		options.markersInfo.insert(std::make_pair(miid, marker));
	}
	return *this;
}

OptionsSetter &OptionsSetter::moveMarkerInfo(Options::MarkerId from,
    Options::MarkerId to)
{
	auto idTo = options.getMarkerInfoId(to);
	auto idFrom = options.getMarkerInfoId(from);
	if (idFrom.has_value() && !idTo.has_value()) {
		auto iter = options.markersInfo.find(*idFrom);
		iter->second = to;
	}
	return *this;
}

OptionsSetter &OptionsSetter::deleteMarkerInfo(
    Options::MarkerId marker)
{
	auto miid = options.getMarkerInfoId(marker);
	if (miid.has_value()) options.markersInfo.erase(*miid);
	return *this;
}

OptionsSetter &OptionsSetter::showTooltip(
    std::optional<Options::MarkerId> marker)
{
	auto current = options.tooltip;
	if (!marker.has_value() && current.has_value()) {
		deleteMarkerInfo(*current);
		options.tooltip.reset();
	}
	else if (marker.has_value() && !current.has_value()) {
		addMarkerInfo(*marker);
		options.tooltip = marker;
	}
	else if (marker.has_value() && current.has_value()
	         && marker != current) {
		moveMarkerInfo(*current, *marker);
		options.tooltip = marker;
	}
	return *this;
}

}