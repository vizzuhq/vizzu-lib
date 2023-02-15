#include "optionssetter.h"

#include <cmath>

#include "base/io/log.h"
#include "base/text/smartstring.h"
#include "data/table/datatable.h"

using namespace Vizzu;
using namespace Vizzu::Diag;

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

OptionsSetter &OptionsSetter::addSeries(const ScaleId &scaleId,
    const std::string &seriesName,
    std::optional<size_t> pos)
{
	if (Text::SmartString::startsWith(seriesName, "$"))
	{
		auto typeStr = seriesName.substr(1, std::string::npos);
		auto type = Data::SeriesType::fromString(typeStr);
		addSeries(scaleId, Data::SeriesIndex(type), pos);
	}
	else if (table)
	{
		auto index = table->getIndex(seriesName);
		addSeries(scaleId, index, pos);
	}
	else
		throw std::logic_error("no table set for options");

	return *this;
}

OptionsSetter &OptionsSetter::deleteSeries(const ScaleId &scaleId,
    const std::string &seriesName)
{
	if (Text::SmartString::startsWith(seriesName, "$"))
	{
		auto typeStr = seriesName.substr(1, std::string::npos);
		auto type = Data::SeriesType::fromString(typeStr);
		deleteSeries(scaleId, Data::SeriesIndex(type));
	}
	else if (table)
	{
		auto index = table->getIndex(seriesName);
		deleteSeries(scaleId, index);
	}
	else
		throw std::logic_error("no table set for options");

	return *this;
}

OptionsSetter &OptionsSetter::addSeries(const ScaleId &scaleId,
										const Data::SeriesIndex &index,
										std::optional<size_t> pos)
{
	options.markersInfo.ref().clear();
	auto res = options.getScales().addSeries(scaleId, index, pos);
	changed |= res.first;
	if (res.first && res.second && onContinousReplaced)
		onContinousReplaced(scaleId, *res.second);
	return *this;
}

OptionsSetter &OptionsSetter::deleteSeries(const ScaleId &scaleId, const Data::SeriesIndex &index)
{
	options.markersInfo.ref().clear();
	changed |= options.getScales().removeSeries(scaleId, index);
	return *this;
}

OptionsSetter &OptionsSetter::clearSeries(const ScaleId &scaleId)
{
	options.markersInfo.ref().clear();
	changed |= options.getScales().clearSeries(scaleId);
	return *this;
}

OptionsSetter &OptionsSetter::setShape(const ShapeType::Type &type)
{
	changed |= options.shapeType.set(type);
	return *this;
}

OptionsSetter &OptionsSetter::setAlign(const Base::Align::Type &alignType)
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
	changed |= options.angle.set(options.angle.get() + ccwQuadrant * M_PI / 2);
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

OptionsSetter &OptionsSetter::setLabelLevel(const ScaleId &scaleId,
    int level)
{
	auto &scale = options.getScales().at(scaleId);
	changed |= scale.labelLevel.set(level);
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

OptionsSetter &OptionsSetter::setRangeMin(const ScaleId &scaleId, 
	const OptionalScaleExtrema &value)
{
	auto &scale = options.getScales().at(scaleId);
	auto act = scale.range.get();
	act.min = value;
	changed |= scale.range.set(act);
	return *this;
}

OptionsSetter &OptionsSetter::setRangeMax(const ScaleId &scaleId, 
	const OptionalScaleExtrema &value)
{
	auto &scale = options.getScales().at(scaleId);
	auto act = scale.range.get();
	act.max = value;
	changed |= scale.range.set(act);
	return *this;
}

OptionsSetter &OptionsSetter::setStackable(const ScaleId &scaleId, bool value)
{
	auto &scale = options.getScales().at(scaleId);
	if (scale.stackable() != value)
	{
		const_cast<bool&>(scale.stackable()) = value;
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

OptionsSetter &OptionsSetter::setTitle(const ScaleId &scaleId,
    const std::string &title)
{
	auto &scale = options.getScales().at(scaleId);
	changed |= scale.title.set(title);
	return *this;
}

OptionsSetter &OptionsSetter::setAxisLine(const ScaleId &scaleId, Base::AutoBool enable)
{
	auto &scale = options.getScales().at(scaleId);
	changed |= scale.axisLine.set(enable);
	return *this;
}

OptionsSetter &OptionsSetter::setAxisLabels(const ScaleId &scaleId, Base::AutoBool enable)
{
	auto &scale = options.getScales().at(scaleId);
	changed |= scale.axisLabels.set(enable);
	return *this;
}

OptionsSetter &OptionsSetter::setTicks(const ScaleId &scaleId, Base::AutoBool enable)
{
	auto &scale = options.getScales().at(scaleId);
	changed |= scale.ticks.set(enable);
	return *this;
}

OptionsSetter &OptionsSetter::setGuides(const ScaleId &scaleId, Base::AutoBool enable)
{
	auto &scale = options.getScales().at(scaleId);
	changed |= scale.guides.set(enable);
	return *this;
}

OptionsSetter &OptionsSetter::setMarkerGuides(const ScaleId &scaleId, Base::AutoBool enable)
{
	auto &scale = options.getScales().at(scaleId);
	changed |= scale.markerGuides.set(enable);
	return *this;
}

OptionsSetter &OptionsSetter::setInterlacing(const ScaleId &scaleId, Base::AutoBool enable)
{
	auto &scale = options.getScales().at(scaleId);
	changed |= scale.interlacing.set(enable);
	return *this;
}

void OptionsSetter::replaceOptions(const Options &options)
{
	if (options != this->options) {
		this->options = options;
		changed = true;
	}
}

OptionsSetter& OptionsSetter::addMarkerInfo(Options::MarkerId mid) {
	if (options.getMarkerInfoId(mid) == Options::nullMarkerInfoId) {
		auto miid = options.generateMarkerInfoId();
		options.markersInfo.ref().insert(std::make_pair(miid, mid));
	}
	return *this;
}

OptionsSetter& OptionsSetter::moveMarkerInfo(Options::MarkerId from, Options::MarkerId to)
{
	auto idTo = options.getMarkerInfoId(to);
	auto idFrom = options.getMarkerInfoId(from);
	if (idFrom != Options::nullMarkerInfoId && idTo == Options::nullMarkerInfoId)
	{
		auto iter = options.markersInfo.ref().find(idFrom);
		iter->second = to;
	}
	return *this;
}

OptionsSetter& OptionsSetter::deleteMarkerInfo(Options::MarkerId mid) {
	auto miid = options.getMarkerInfoId(mid);
	if (miid != Options::nullMarkerInfoId)
		options.markersInfo.ref().erase(miid);
	return *this;
}

OptionsSetter& OptionsSetter::showTooltip(Options::MarkerId mid) {
	auto current = options.tooltipId.get();
	if (mid == Options::nullMarkerId && current != Options::nullMarkerInfoId) {
		deleteMarkerInfo(options.tooltipId.get());
		options.tooltipId.set(Options::nullMarkerInfoId);
	}
	else if (mid != Options::nullMarkerId && current == Options::nullMarkerId) {
		addMarkerInfo(mid);
		options.tooltipId.set(mid);
	}
	else if (mid != Options::nullMarkerId && current != Options::nullMarkerId && mid != current) {
		moveMarkerInfo(options.tooltipId.get(), mid);
		options.tooltipId.set(mid);
	}
	return *this;
}
