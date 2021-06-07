#include "optionssetter.h"

#include <cmath>

#include "base/io/log.h"
#include "base/text/smartstring.h"
#include "data/table/datatable.h"

using namespace Vizzu;
using namespace Vizzu::Diag;

OptionsSetter::OptionsSetter(DiagramOptions &options,
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
	auto res = options.getScales().addSeries(scaleId, index, pos);
	changed |= res.first;
	if (res.first && res.second && onContinousReplaced)
		onContinousReplaced(scaleId, *res.second);
	return *this;
}

OptionsSetter &OptionsSetter::deleteSeries(const Scales::Id &scaleId, const Data::SeriesIndex &index)
{
	changed |= options.getScales().removeSeries(scaleId, index);
	return *this;
}

OptionsSetter &OptionsSetter::clearSeries(const Scales::Id &scaleId)
{
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

OptionsSetter &OptionsSetter::addToFilter(const Data::DataFilter::AndConditions &conditions)
{
	changed |= options.dataFilters.ref().top().addConditions(conditions);
	return *this;
}

OptionsSetter &OptionsSetter::setFilter(const Data::DataFilter &filter)
{
	if (!(options.dataFilters.get().top() == filter))
	{
		options.dataFilters.ref().top() = filter;
		changed |= 1;
	}
	return *this;
}

OptionsSetter &OptionsSetter::delFromFilter(const Data::DataFilter::AndConditions &conditions)
{
	for (auto &filter : options.dataFilters.ref())
		changed |= filter.removeConditions(conditions);
	return *this;
}

OptionsSetter &OptionsSetter::setFilterCondition(
	int orIndex,
    const std::string &key,
    const std::string &value)
{
	if (table)
	{
		Data::DataFilter::Condition condition(*table, key, value);
		changed |=
		    options.dataFilters.ref().top().addCondition(orIndex, condition);
		return *this;
	}
	else
		throw std::logic_error("no table set for options");
}

OptionsSetter &OptionsSetter::delFromFilter(const Data::DataFilter::Condition &condition)
{
	for (auto &filter : options.dataFilters.ref())
		changed |= filter.removeCondition(condition);
	return *this;
}

OptionsSetter &OptionsSetter::clearFilter()
{
	changed |= options.dataFilters.ref().top().clear();
	return *this;
}

OptionsSetter &OptionsSetter::pushFilter()
{
	changed |= options.dataFilters.ref().pushNew();
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

void OptionsSetter::replaceOptions(const DiagramOptions &options)
{
	if (options != this->options) {
		this->options = options;
		changed = true;
	}
}
