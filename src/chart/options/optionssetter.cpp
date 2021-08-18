#include "optionssetter.h"

#include <cmath>

#include "base/io/log.h"
#include "base/text/smartstring.h"
#include "data/table/datatable.h"

using namespace Vizzu;
using namespace Vizzu::Diag;

OptionsSetter::OptionsSetter(Options &options,
    const OnFinished::Listener &onFinished) :
    options(options), currentTooltipId(Options::nullMarkerId)
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

OptionsSetter &OptionsSetter::addSeries(const Scales::Id &scaleId,
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

OptionsSetter &OptionsSetter::deleteSeries(const Scales::Id &scaleId,
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

OptionsSetter &OptionsSetter::addSeries(const Scales::Id &scaleId,
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

OptionsSetter &OptionsSetter::deleteSeries(const Scales::Id &scaleId, const Data::SeriesIndex &index)
{
	options.markersInfo.ref().clear();
	changed |= options.getScales().removeSeries(scaleId, index);
	return *this;
}

OptionsSetter &OptionsSetter::clearSeries(const Scales::Id &scaleId)
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

OptionsSetter &OptionsSetter::setLabelLevel(const Scales::Id &scaleId,
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

OptionsSetter &OptionsSetter::setRange(const Scales::Id &scaleId, Type::PhysicalValue<Math::Range<double> > range)
{
	auto &scale = options.getScales().at(scaleId);
	changed |= scale.range.set(range);
	return *this;
}

OptionsSetter &OptionsSetter::setStackable(const Scales::Id &scaleId, bool value)
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

OptionsSetter &OptionsSetter::setLegend(std::optional<Scale::Type> type)
{
	changed = options.legend.set(type);
	return *this;
}

OptionsSetter &OptionsSetter::setTitle(const Scales::Id &scaleId,
    const std::string &title)
{
	auto &scale = options.getScales().at(scaleId);
	changed |= scale.title.set(title);
	return *this;
}

OptionsSetter &OptionsSetter::setBubbleChartAlgorithm(
    BubbleChartAlgorithm value)
{
	changed = options.bubbleChartAlgorithm.set(value);
	return *this;
}

void OptionsSetter::replaceOptions(const Options &options)
{
	if (options != this->options) {
		this->options = options;
		changed = true;
	}
}

OptionsSetter& OptionsSetter::addMarkerInfo(const Options::MarkerIndex& index) {
	if (options.markersInfoIdFromIndex(index) == Options::nullMarkerId) {
		auto id = options.generateMarkersInfoId();
		options.markersInfo.ref().insert(std::make_pair(id, index));
	}
	return *this;
}

OptionsSetter& OptionsSetter::moveMarkerInfo(const Options::MarkerIndex& from,
	const Options::MarkerIndex& to)
{
	auto idTo = options.markersInfoIdFromIndex(to);
	auto idFrom = options.markersInfoIdFromIndex(from);
	if (idFrom != Options::nullMarkerId && idTo == Options::nullMarkerId)
	{
		auto iter = options.markersInfo.ref().find(idFrom);
		iter->second = to;
	}
	return *this;
}

OptionsSetter& OptionsSetter::deleteMarkerInfo(const Options::MarkerIndex& index) {
	auto id = options.markersInfoIdFromIndex(index);
	if (id != Options::nullMarkerId)
		options.markersInfo.ref().erase(id);
	return *this;
}

OptionsSetter& OptionsSetter::showTooltip(uint64_t) {
	//if (id == Options::nullMarkerId && currentTooltipId != Options::nullMarkerId) {
	//	options.markersInfo.ref().erase(id);
	//	currentTooltipId = Options::nullMarkerId;
	//}
	//if (id != Options::nullMarkerId && currentTooltipId == Options::nullMarkerId) {
	//	options.markersInfo.ref().erase(id);
	//	currentTooltipId = id;
	//}
	return *this;
}
