#ifndef OPTIONSDECORATOR_H
#define OPTIONSDECORATOR_H

#include "optionssetter.h"

namespace Vizzu
{
namespace Diag
{

class OptionsDecorator : public OptionsSetter
{
public:
	explicit OptionsDecorator(OptionsSetter &setter) :
	    OptionsSetter(setter.getOptions()),
		setter(setter)
	{}

	OptionsSetter &addSeries(const Scales::Id &scaleId,
	    const Data::SeriesIndex &index,
	    std::optional<size_t> pos = std::nullopt) override
	{
		setter.addSeries(scaleId, index, pos);
		return *this;
	}

	OptionsSetter &deleteSeries(const Scales::Id &scaleId,
	    const Data::SeriesIndex &index) override
	{
		setter.deleteSeries(scaleId, index);
		return *this;
	}

	OptionsSetter &setShape(const ShapeType::Type &type) override
	{
		setter.setShape(type);
		return *this;
	}

	OptionsSetter &setAlign(
	    const Base::Align::Type &alignType) override
	{
		setter.setAlign(alignType);
		return *this;
	}

	OptionsSetter &setPolar(bool value) override
	{
		setter.setPolar(value);
		return *this;
	}

	OptionsSetter &setSplitted(bool value) override
	{
		setter.setSplitted(value);
		return *this;
	}

	OptionsSetter &rotate(double ccwQuadrant) override
	{
		setter.rotate(ccwQuadrant);
		return *this;
	}

	OptionsSetter &setAngle(double ccwQuadrant) override
	{
		setter.setAngle(ccwQuadrant);
		return *this;
	}

	OptionsSetter &setHorizontal(bool horizontal) override
	{
		setter.setHorizontal(horizontal);
		return *this;
	}

	OptionsSetter &addToFilter(
	    const Data::DataFilter::AndConditions &conditions) override
	{
		setter.addToFilter(conditions);
		return *this;
	}

	OptionsSetter &setFilter(const Data::DataFilter &filter) override
	{
		setter.setFilter(filter);
		return *this;
	}

	OptionsSetter &delFromFilter(
	    const Data::DataFilter::AndConditions &conditions) override
	{
		setter.delFromFilter(conditions);
		return *this;
	}

	OptionsSetter &setFilterCondition(int orIndex,
	    const std::string &key,
	    const std::string &value) override
	{
		setter.setFilterCondition(orIndex, key, value);
		return *this;
	}

	OptionsSetter &delFromFilter(
	    const Data::DataFilter::Condition &condition) override
	{
		setter.delFromFilter(condition);
		return *this;
	}

	OptionsSetter &clearFilter() override
	{
		setter.clearFilter();
		return *this;
	}

	OptionsSetter &pushFilter() override
	{
		setter.pushFilter();
		return *this;
	}

	OptionsSetter &setLabelLevel(const Scales::Id &scaleId,
	    int level) override
	{
		setter.setLabelLevel(scaleId, level);
		return *this;
	}

	OptionsSetter &setSorted(bool value) override
	{
		setter.setSorted(value);
		return *this;
	}

	OptionsSetter &setReverse(bool value) override
	{
		setter.setReverse(value);
		return *this;
	}

	OptionsSetter &setRange(const Scales::Id &scaleId,
	    Type::PhysicalValue<Math::Range<double>> range) override
	{
		setter.setRange(scaleId, range);
		return *this;
	}

	OptionsSetter &setStackable(const Scales::Id &scaleId,
	    bool value) override
	{
		setter.setStackable(scaleId, value);
		return *this;
	}

	OptionsSetter &setTitle(const std::optional<std::string> &title) override
	{
		setter.setTitle(title);
		return *this;
	}

	OptionsSetter &setLegend(std::optional<Scale::Type> type) override
	{
		setter.setLegend(type);
		return *this;
	}

	OptionsSetter &setBubbleChartAlgorithm(
	    BubbleChartAlgorithm value) override
	{
		setter.setBubbleChartAlgorithm(value);
		return *this;
	}

	void replaceOptions(const DiagramOptions &options) override {
		setter.replaceOptions(options);
	}

protected:
	OptionsSetter &setter;
};

}
}

#endif
