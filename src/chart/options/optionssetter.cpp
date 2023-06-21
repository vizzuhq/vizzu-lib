#include "optionssetter.h"

#include <cmath>

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
	options.markersInfo.ref().clear();
	auto res = options.getChannels().addSeries(channelId, index, pos);
	changed |= res.first;
	if (res.first && res.second && onMeasureReplaced)
		onMeasureReplaced(channelId, *res.second);
	return *this;
}

OptionsSetter &OptionsSetter::deleteSeries(const ChannelId &channelId,
    const Data::SeriesIndex &index)
{
	options.markersInfo.ref().clear();
	changed |= options.getChannels().removeSeries(channelId, index);
	return *this;
}

OptionsSetter &OptionsSetter::clearSeries(const ChannelId &channelId)
{
	options.markersInfo.ref().clear();
	changed |= options.getChannels().clearSeries(channelId);
	return *this;
}

OptionsSetter &OptionsSetter::setShape(const ShapeType &type)
{
	changed |= options.shapeType.set(type);
	return *this;
}

OptionsSetter &OptionsSetter::setAlign(
    const Base::Align::Type &alignType)
{
	changed |= options.alignType.set(alignType);
	return *this;
}

OptionsSetter &OptionsSetter::setPolar(bool value)
{
	changed |= options.polar.set(Math::FuzzyBool(value));
	return *this;
}

OptionsSetter &OptionsSetter::setSplitted(bool value)
{
	changed |= options.splitted.set(Math::FuzzyBool(value));
	return *this;
}

OptionsSetter &OptionsSetter::rotate(double ccwQuadrant)
{
	changed |= options.angle.set(
	    options.angle.get() + ccwQuadrant * M_PI / 2);
	return *this;
}

OptionsSetter &OptionsSetter::setAngle(double ccwQuadrant)
{
	changed |= options.angle.set(ccwQuadrant * M_PI / 2);
	return *this;
}

OptionsSetter &OptionsSetter::setHorizontal(bool horizontal)
{
	changed |= options.horizontal.set(Math::FuzzyBool(horizontal));
	return *this;
}

OptionsSetter &OptionsSetter::setFilter(const Data::Filter &filter)
{
	changed |= options.dataFilter.set(filter);
	return *this;
}

OptionsSetter &OptionsSetter::setLabelLevel(const ChannelId &channelId,
    int level)
{
	auto &channel = options.getChannels().at(channelId);
	changed |= channel.labelLevel.set(level);
	return *this;
}

OptionsSetter &OptionsSetter::setSorted(bool value)
{
	changed |= options.sorted.set(value);
	return *this;
}

OptionsSetter &OptionsSetter::setReverse(bool value)
{
	changed |= options.reverse.set(value);
	return *this;
}

OptionsSetter &OptionsSetter::setRangeMin(const ChannelId &channelId,
    const OptionalChannelExtrema &value)
{
	auto &channel = options.getChannels().at(channelId);
	auto act = channel.range.get();
	act.min = value;
	changed |= channel.range.set(act);
	return *this;
}

OptionsSetter &OptionsSetter::setRangeMax(const ChannelId &channelId,
    const OptionalChannelExtrema &value)
{
	auto &channel = options.getChannels().at(channelId);
	auto act = channel.range.get();
	act.max = value;
	changed |= channel.range.set(act);
	return *this;
}

OptionsSetter &OptionsSetter::setStackable(const ChannelId &channelId,
    bool value)
{
	auto &channel = options.getChannels().at(channelId);
	if (channel.stackable() != value) {
		channel.setStackable(value);
		changed = true;
	}
	return *this;
}

OptionsSetter &OptionsSetter::setTitle(
    const std::optional<std::string> &title)
{
	changed = options.title.set(title);
	return *this;
}

OptionsSetter &OptionsSetter::setLegend(const Options::Legend &legend)
{
	changed = options.legend.set(legend);
	return *this;
}

OptionsSetter &OptionsSetter::setTitle(const ChannelId &channelId,
    const std::string &title)
{
	auto &channel = options.getChannels().at(channelId);
	changed |= channel.title.set(title);
	return *this;
}

OptionsSetter &OptionsSetter::setAxisLine(const ChannelId &channelId,
    Base::AutoBool enable)
{
	auto &channel = options.getChannels().at(channelId);
	changed |= channel.axisLine.set(enable);
	return *this;
}

OptionsSetter &OptionsSetter::setAxisLabels(const ChannelId &channelId,
    Base::AutoBool enable)
{
	auto &channel = options.getChannels().at(channelId);
	changed |= channel.axisLabels.set(enable);
	return *this;
}

OptionsSetter &OptionsSetter::setTicks(const ChannelId &channelId,
    Base::AutoBool enable)
{
	auto &channel = options.getChannels().at(channelId);
	changed |= channel.ticks.set(enable);
	return *this;
}

OptionsSetter &OptionsSetter::setGuides(const ChannelId &channelId,
    Base::AutoBool enable)
{
	auto &channel = options.getChannels().at(channelId);
	changed |= channel.guides.set(enable);
	return *this;
}

OptionsSetter &OptionsSetter::setMarkerGuides(const ChannelId &channelId,
    Base::AutoBool enable)
{
	auto &channel = options.getChannels().at(channelId);
	changed |= channel.markerGuides.set(enable);
	return *this;
}

OptionsSetter &OptionsSetter::setInterlacing(const ChannelId &channelId,
    Base::AutoBool enable)
{
	auto &channel = options.getChannels().at(channelId);
	changed |= channel.interlacing.set(enable);
	return *this;
}

OptionsSetter &OptionsSetter::setStep(const ChannelId &channelId,
    Base::AutoParam<double> step)
{
	auto &channel = options.getChannels().at(channelId);
	changed |= channel.step.set(step);
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
		options.markersInfo.ref().insert(std::make_pair(miid, mid));
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
		auto iter = options.markersInfo.ref().find(idFrom);
		iter->second = to;
	}
	return *this;
}

OptionsSetter &OptionsSetter::deleteMarkerInfo(Options::MarkerId mid)
{
	auto miid = options.getMarkerInfoId(mid);
	if (miid != Options::nullMarkerInfoId)
		options.markersInfo.ref().erase(miid);
	return *this;
}

OptionsSetter &OptionsSetter::showTooltip(Options::MarkerId mid)
{
	auto current = options.tooltipId.get();
	if (mid == Options::nullMarkerId
	    && current != Options::nullMarkerInfoId) {
		deleteMarkerInfo(options.tooltipId.get());
		options.tooltipId.set(Options::nullMarkerInfoId);
	}
	else if (mid != Options::nullMarkerId
	         && current == Options::nullMarkerId) {
		addMarkerInfo(mid);
		options.tooltipId.set(mid);
	}
	else if (mid != Options::nullMarkerId
	         && current != Options::nullMarkerId && mid != current) {
		moveMarkerInfo(options.tooltipId.get(), mid);
		options.tooltipId.set(mid);
	}
	return *this;
}
