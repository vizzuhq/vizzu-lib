#ifndef OPTIONSDECORATOR_H
#define OPTIONSDECORATOR_H

#include "optionssetter.h"

namespace Vizzu
{
namespace Gen
{

class OptionsDecorator : public OptionsSetter
{
public:
	explicit OptionsDecorator(OptionsSetter &setter) :
	    OptionsSetter(setter.getOptions()),
	    setter(setter)
	{}

	OptionsSetter &addSeries(const ChannelId &channelId,
	    const Data::SeriesIndex &index,
	    std::optional<size_t> pos = std::nullopt) override
	{
		setter.addSeries(channelId, index, pos);
		return *this;
	}

	OptionsSetter &deleteSeries(const ChannelId &channelId,
	    const Data::SeriesIndex &index) override
	{
		setter.deleteSeries(channelId, index);
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

	OptionsSetter &setLabelLevel(const ChannelId &channelId,
	    int level) override
	{
		setter.setLabelLevel(channelId, level);
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

	OptionsSetter &setRangeMin(const ChannelId &channelId,
	    const OptionalChannelExtrema &value) override
	{
		setter.setRangeMin(channelId, value);
		return *this;
	}

	OptionsSetter &setRangeMax(const ChannelId &channelId,
	    const OptionalChannelExtrema &value) override
	{
		setter.setRangeMax(channelId, value);
		return *this;
	}

	OptionsSetter &setStackable(const ChannelId &channelId,
	    bool value) override
	{
		setter.setStackable(channelId, value);
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
