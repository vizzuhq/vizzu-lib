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

OptionsSetter &OptionsSetter::setFilter(const Data::Filter &filter)
{
	options.dataFilter = filter;
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