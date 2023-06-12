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

	OptionsSetter &addSeries(const ScaleId &scaleId,
	    const Data::SeriesIndex &index,
	    std::optional<size_t> pos = std::nullopt) override
	{
		setter.addSeries(scaleId, index, pos);
		return *this;
	}

	OptionsSetter &deleteSeries(const ScaleId &scaleId,
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

	OptionsSetter &setFilter(const Data::Filter &filter) override
	{
		setter.setFilter(filter);
		return *this;
	}

	OptionsSetter &setLabelLevel(const ScaleId &scaleId,
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

	OptionsSetter &setRangeMin(const ScaleId &scaleId,
	    const OptionalScaleExtrema &value) override
	{
		setter.setRangeMin(scaleId, value);
		return *this;
	}

	OptionsSetter &setRangeMax(const ScaleId &scaleId,
	    const OptionalScaleExtrema &value) override
	{
		setter.setRangeMax(scaleId, value);
		return *this;
	}

	OptionsSetter &setStackable(const ScaleId &scaleId,
	    bool value) override
	{
		setter.setStackable(scaleId, value);
		return *this;
	}

	OptionsSetter &setTitle(
	    const std::optional<std::string> &title) override
	{
		setter.setTitle(title);
		return *this;
	}

	OptionsSetter &setLegend(const Options::Legend &legend) override
	{
		setter.setLegend(legend);
		return *this;
	}

	void replaceOptions(const Options &options) override
	{
		setter.replaceOptions(options);
	}

protected:
	OptionsSetter &setter;
};

}
}

#endif
