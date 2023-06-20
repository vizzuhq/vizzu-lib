#include "optionssetter.h"

#include <cmath>
#include <utility>

#include "base/io/log.h"
#include "base/text/smartstring.h"
#include "data/table/datatable.h"

using namespace Vizzu;
using namespace Vizzu::Gen;

OptionsSetter::OptionsSetter(Options &options,
    const OnFinished::Listener &onFinished) :
    options(options)
{
	if (onFinished) this->onFinished.attach(onFinished);
	changed = false;
	table = nullptr;
}

OptionsSetter::~OptionsSetter()
{
	if (changed) onFinished();
}

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
	auto res = options.getChannels().addSeries(channelId, index, pos);
	changed |= res.first;
	if (res.first && res.second && onMeasureReplaced)
		onMeasureReplaced(channelId, *res.second);
	return *this;
}

OptionsSetter &OptionsSetter::deleteSeries(const ChannelId &channelId,
    const Data::SeriesIndex &index)
{
	options.markersInfo.clear();
	changed |= options.getChannels().removeSeries(channelId, index);
	return *this;
}

OptionsSetter &OptionsSetter::clearSeries(const ChannelId &channelId)
{
	options.markersInfo.clear();
	changed |= options.getChannels().clearSeries(channelId);
	return *this;
}

OptionsSetter &OptionsSetter::setShape(const ShapeType::Type &type)
{
	changed |= (std::exchange(options.shapeType, type) != type);
	return *this;
}

OptionsSetter &OptionsSetter::setAlign(
    const Base::Align::Type &alignType)
{
	changed |= (std::exchange(options.alignType, alignType) != alignType);
	return *this;
}

OptionsSetter &OptionsSetter::setPolar(bool value)
{
	changed |= (std::exchange(options.polar, Math::FuzzyBool(value)) != Math::FuzzyBool(value));
	return *this;
}

OptionsSetter &OptionsSetter::setSplitted(bool value)
{
	changed |= (std::exchange(options.splitted, Math::FuzzyBool(value)) != Math::FuzzyBool(value));
	return *this;
}

OptionsSetter &OptionsSetter::rotate(double ccwQuadrant)
{
	changed |= (std::exchange(options.angle,
	    options.angle + ccwQuadrant * M_PI / 2)) != options.angle + ccwQuadrant * M_PI / 2;
	return *this;
}

OptionsSetter &OptionsSetter::setAngle(double ccwQuadrant)
{
	changed |= (std::exchange(options.angle, ccwQuadrant * M_PI / 2) != ccwQuadrant * M_PI / 2);
	return *this;
}

OptionsSetter &OptionsSetter::setHorizontal(bool horizontal)
{
	changed |= (std::exchange(options.horizontal, Math::FuzzyBool(horizontal)) != Math::FuzzyBool(horizontal));
	return *this;
}

OptionsSetter &OptionsSetter::setFilter(const Data::Filter &filter)
{
	changed |= (std::exchange(options.dataFilter, filter) != filter);
	return *this;
}

OptionsSetter &OptionsSetter::setLabelLevel(const ChannelId &channelId,
    int level)
{
	auto &channel = options.getChannels().at(channelId);
	changed |= (std::exchange(channel.labelLevel, level) != level);
	return *this;
}

OptionsSetter &OptionsSetter::setSorted(bool value)
{
	changed |= (std::exchange(options.sorted, value) != value);
	return *this;
}

OptionsSetter &OptionsSetter::setReverse(bool value)
{
	changed |= (std::exchange(options.reverse, value) != value);
	return *this;
}

OptionsSetter &OptionsSetter::setRangeMin(const ChannelId &channelId,
    const OptionalChannelExtrema &value)
{
	auto &channel = options.getChannels().at(channelId);
	auto act = channel.range;
	act.min = value;
	changed |= (std::exchange(channel.range, act) != act);
	return *this;
}

OptionsSetter &OptionsSetter::setRangeMax(const ChannelId &channelId,
    const OptionalChannelExtrema &value)
{
	auto &channel = options.getChannels().at(channelId);
	auto act = channel.range;
	act.max = value;
	changed |= (std::exchange(channel.range, act) != act);
	return *this;
}

OptionsSetter &OptionsSetter::setStackable(const ChannelId &channelId,
    bool value)
{
	auto &channel = options.getChannels().at(channelId);
	if (channel.stackable != value) {
		channel.stackable = value;
		changed = true;
	}
	return *this;
}

OptionsSetter &OptionsSetter::setTitle(
    const std::optional<std::string> &title)
{
	changed = (std::exchange(options.title, title) != title);
	return *this;
}

OptionsSetter &OptionsSetter::setLegend(const Options::Legend &legend)
{
	changed = (std::exchange(options.legend, legend) != legend);
	return *this;
}

OptionsSetter &OptionsSetter::setTitle(const ChannelId &channelId,
    const std::string &title)
{
	auto &channel = options.getChannels().at(channelId);
	changed |= (std::exchange(channel.title, title) != title);
	return *this;
}

OptionsSetter &OptionsSetter::setAxisLine(const ChannelId &channelId,
    Base::AutoBool enable)
{
	auto &channel = options.getChannels().at(channelId);
	changed |= (std::exchange(channel.axisLine, enable) != enable);
	return *this;
}

OptionsSetter &OptionsSetter::setAxisLabels(const ChannelId &channelId,
    Base::AutoBool enable)
{
	auto &channel = options.getChannels().at(channelId);
	changed |= (std::exchange(channel.axisLabels, enable) != enable);
	return *this;
}

OptionsSetter &OptionsSetter::setTicks(const ChannelId &channelId,
    Base::AutoBool enable)
{
	auto &channel = options.getChannels().at(channelId);
	changed |= (std::exchange(channel.ticks, enable) != enable);
	return *this;
}

OptionsSetter &OptionsSetter::setGuides(const ChannelId &channelId,
    Base::AutoBool enable)
{
	auto &channel = options.getChannels().at(channelId);
	changed |= (std::exchange(channel.guides, enable) != enable);
	return *this;
}

OptionsSetter &OptionsSetter::setMarkerGuides(const ChannelId &channelId,
    Base::AutoBool enable)
{
	auto &channel = options.getChannels().at(channelId);
	changed |= (std::exchange(channel.markerGuides, enable) != enable);
	return *this;
}

OptionsSetter &OptionsSetter::setInterlacing(const ChannelId &channelId,
    Base::AutoBool enable)
{
	auto &channel = options.getChannels().at(channelId);
	changed |= (std::exchange(channel.interlacing, enable) != enable);
	return *this;
}

OptionsSetter &OptionsSetter::setStep(const ChannelId &channelId,
    Base::AutoParam<double> step)
{
	auto &channel = options.getChannels().at(channelId);
	changed |= (std::exchange(channel.step, step) != step);
	return *this;
}

void OptionsSetter::replaceOptions(const Options &options)
{
	if (options != this->options) {
		this->options = options;
		changed = true;
	}
}

OptionsSetter &OptionsSetter::addMarkerInfo(Options::MarkerId mid)
{
	if (options.getMarkerInfoId(mid) == Options::nullMarkerInfoId) {
		auto miid = options.generateMarkerInfoId();
		options.markersInfo.insert(std::make_pair(miid, mid));
	}
	return *this;
}

OptionsSetter &OptionsSetter::moveMarkerInfo(Options::MarkerId from,
    Options::MarkerId to)
{
	auto idTo = options.getMarkerInfoId(to);
	auto idFrom = options.getMarkerInfoId(from);
	if (idFrom != Options::nullMarkerInfoId
	    && idTo == Options::nullMarkerInfoId) {
		auto iter = options.markersInfo.find(idFrom);
		iter->second = to;
	}
	return *this;
}

OptionsSetter &OptionsSetter::deleteMarkerInfo(Options::MarkerId mid)
{
	auto miid = options.getMarkerInfoId(mid);
	if (miid != Options::nullMarkerInfoId)
		options.markersInfo.erase(miid);
	return *this;
}

OptionsSetter &OptionsSetter::showTooltip(Options::MarkerId mid)
{
	auto current = options.tooltipId;
	if (mid == Options::nullMarkerId
	    && current != Options::nullMarkerInfoId) {
		deleteMarkerInfo(options.tooltipId);
		options.tooltipId = Options::nullMarkerInfoId;
	}
	else if (mid != Options::nullMarkerId
	         && current == Options::nullMarkerId) {
		addMarkerInfo(mid);
		options.tooltipId = mid;
	}
	else if (mid != Options::nullMarkerId
	         && current != Options::nullMarkerId && mid != current) {
		moveMarkerInfo(options.tooltipId, mid);
		options.tooltipId = mid;
	}
	return *this;
}
