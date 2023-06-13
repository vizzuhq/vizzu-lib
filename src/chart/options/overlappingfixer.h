#ifndef CHART_OPTIONS_OVERLAPPINGFIXER_H
#define CHART_OPTIONS_OVERLAPPINGFIXER_H

#include "optionsdecorator.h"

namespace Vizzu
{
namespace Gen
{

class OverlappingFixer : public OptionsDecorator
{
public:
	using OptionsDecorator::OptionsDecorator;

	bool enableOverlap = false;

	OptionsSetter &addSeries(const ChannelId &channelId,
	    const Data::SeriesIndex &index,
	    std::optional<size_t> pos = std::nullopt) override;
	OptionsSetter &deleteSeries(const ChannelId &channelId,
	    const Data::SeriesIndex &index) override;

	OptionsSetter &setShape(const ShapeType::Type &type) override;
	OptionsSetter &setHorizontal(bool horizontal) override;

private:
	void removeOverlap(bool byDelete);
	void fixOverlap(bool byDelete, ShapeType::Type type);
	void removeStack(bool byDelete);
};

}
}

#endif
