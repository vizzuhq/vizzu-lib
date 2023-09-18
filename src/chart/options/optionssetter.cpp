#include "optionssetter.h"

#include <cmath>
#include <utility>

#include "base/text/smartstring.h"
#include "data/table/datatable.h"

Vizzu::Gen::OptionsSetter::OptionsSetter(Options &options) :
    options(options)
{}

void Vizzu::Gen::OptionsSetter::setTable(
    const Vizzu::Data::DataTable *table)
{
	this->table = table;
}

Vizzu::Gen::OptionsSetter &Vizzu::Gen::OptionsSetter::addSeries(
    const ChannelId &channelId,
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

Vizzu::Gen::OptionsSetter &Vizzu::Gen::OptionsSetter::deleteSeries(
    const ChannelId &channelId,
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

Vizzu::Gen::OptionsSetter &Vizzu::Gen::OptionsSetter::addSeries(
    const ChannelId &channelId,
    const Data::SeriesIndex &index,
    std::optional<size_t> pos)
{
	options.markersInfo.clear();
	options.getChannels().addSeries(channelId, index, pos);
	return *this;
}

Vizzu::Gen::OptionsSetter &Vizzu::Gen::OptionsSetter::deleteSeries(
    const ChannelId &channelId,
    const Data::SeriesIndex &index)
{
	options.markersInfo.clear();
	options.getChannels().removeSeries(channelId, index);
	return *this;
}

Vizzu::Gen::OptionsSetter &Vizzu::Gen::OptionsSetter::clearSeries(
    const ChannelId &channelId)
{
	options.markersInfo.clear();
	options.getChannels().clearSeries(channelId);
	return *this;
}

Vizzu::Gen::OptionsSetter &Vizzu::Gen::OptionsSetter::setShape(
    const ShapeType &type)
{
	options.geometry = type;
	return *this;
}

Vizzu::Gen::OptionsSetter &Vizzu::Gen::OptionsSetter::setAlign(
    const Base::Align::Type &alignType)
{
	options.align = alignType;
	return *this;
}

Vizzu::Gen::OptionsSetter &Vizzu::Gen::OptionsSetter::setCoordSystem(
    CoordSystem coordSystem)
{
	options.coordSystem = coordSystem;
	return *this;
}

Vizzu::Gen::OptionsSetter &Vizzu::Gen::OptionsSetter::setSplitted(
    bool value)
{
	options.split = Math::FuzzyBool(value);
	return *this;
}

Vizzu::Gen::OptionsSetter &Vizzu::Gen::OptionsSetter::rotate(
    double ccwQuadrant)
{
	options.angle += ccwQuadrant * M_PI / 2;
	return *this;
}

Vizzu::Gen::OptionsSetter &Vizzu::Gen::OptionsSetter::setAngle(
    double ccwQuadrant)
{
	options.angle = ccwQuadrant * M_PI / 2;
	return *this;
}

Vizzu::Gen::OptionsSetter &Vizzu::Gen::OptionsSetter::setOrientation(
    Options::OrientationType value)
{
	options.orientation = value;
	return *this;
}

Vizzu::Gen::OptionsSetter &Vizzu::Gen::OptionsSetter::setFilter(
    const Data::Filter &filter)
{
	options.dataFilter = filter;
	return *this;
}

Vizzu::Gen::OptionsSetter &Vizzu::Gen::OptionsSetter::setLabelLevel(
    const ChannelId &channelId,
    std::size_t level)
{
	options.getChannels().at(channelId).labelLevel = level;
	return *this;
}

Vizzu::Gen::OptionsSetter &Vizzu::Gen::OptionsSetter::setSorted(
    Sort value)
{
	options.sort = value;
	return *this;
}

Vizzu::Gen::OptionsSetter &Vizzu::Gen::OptionsSetter::setReverse(
    bool value)
{
	options.reverse = value;
	return *this;
}

Vizzu::Gen::OptionsSetter &Vizzu::Gen::OptionsSetter::setRangeMin(
    const ChannelId &channelId,
    const OptionalChannelExtrema &value)
{
	options.getChannels().at(channelId).range.min = value;
	return *this;
}

Vizzu::Gen::OptionsSetter &Vizzu::Gen::OptionsSetter::setRangeMax(
    const ChannelId &channelId,
    const OptionalChannelExtrema &value)
{
	options.getChannels().at(channelId).range.max = value;
	return *this;
}

Vizzu::Gen::OptionsSetter &Vizzu::Gen::OptionsSetter::setStackable(
    const ChannelId &channelId,
    bool value)
{
	options.getChannels().at(channelId).stackable = value;
	return *this;
}

Vizzu::Gen::OptionsSetter &Vizzu::Gen::OptionsSetter::setTitle(
    const std::optional<std::string> &title)
{
	options.title = title;
	return *this;
}

Vizzu::Gen::OptionsSetter &Vizzu::Gen::OptionsSetter::setLegend(
    const Options::LegendType &legend)
{
	options.legend = legend;
	return *this;
}

Vizzu::Gen::OptionsSetter &Vizzu::Gen::OptionsSetter::setAxisTitle(
    const ChannelId &channelId,
    const std::string &title)
{
	options.getChannels().at(channelId).title = title;
	return *this;
}

Vizzu::Gen::OptionsSetter &Vizzu::Gen::OptionsSetter::setAxisLine(
    const ChannelId &channelId,
    Base::AutoBool enable)
{
	options.getChannels().at(channelId).axisLine = enable;
	return *this;
}

Vizzu::Gen::OptionsSetter &Vizzu::Gen::OptionsSetter::setAxisLabels(
    const ChannelId &channelId,
    Base::AutoBool enable)
{
	options.getChannels().at(channelId).axisLabels = enable;
	return *this;
}

Vizzu::Gen::OptionsSetter &Vizzu::Gen::OptionsSetter::setTicks(
    const ChannelId &channelId,
    Base::AutoBool enable)
{
	options.getChannels().at(channelId).ticks = enable;
	return *this;
}

Vizzu::Gen::OptionsSetter &Vizzu::Gen::OptionsSetter::setGuides(
    const ChannelId &channelId,
    Base::AutoBool enable)
{
	options.getChannels().at(channelId).guides = enable;
	return *this;
}

Vizzu::Gen::OptionsSetter &Vizzu::Gen::OptionsSetter::setMarkerGuides(
    const ChannelId &channelId,
    Base::AutoBool enable)
{
	options.getChannels().at(channelId).markerGuides = enable;
	return *this;
}

Vizzu::Gen::OptionsSetter &Vizzu::Gen::OptionsSetter::setInterlacing(
    const ChannelId &channelId,
    Base::AutoBool enable)
{
	options.getChannels().at(channelId).interlacing = enable;
	return *this;
}

Vizzu::Gen::OptionsSetter &Vizzu::Gen::OptionsSetter::setStep(
    const ChannelId &channelId,
    Base::AutoParam<double> step)
{
	options.getChannels().at(channelId).step = step;
	return *this;
}

Vizzu::Gen::OptionsSetter &Vizzu::Gen::OptionsSetter::addMarkerInfo(
    Options::MarkerId marker)
{
	if (!options.getMarkerInfoId(marker).has_value()) {
		auto miid = Options::generateMarkerInfoId();
		options.markersInfo.insert(std::make_pair(miid, marker));
	}
	return *this;
}

Vizzu::Gen::OptionsSetter &Vizzu::Gen::OptionsSetter::moveMarkerInfo(
    Options::MarkerId from,
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

Vizzu::Gen::OptionsSetter &
Vizzu::Gen::OptionsSetter::deleteMarkerInfo(Options::MarkerId marker)
{
	auto miid = options.getMarkerInfoId(marker);
	if (miid.has_value()) options.markersInfo.erase(*miid);
	return *this;
}

Vizzu::Gen::OptionsSetter &Vizzu::Gen::OptionsSetter::showTooltip(
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
