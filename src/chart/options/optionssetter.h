#ifndef OPTIONSSETTER_H
#define OPTIONSSETTER_H

#include "base/util/event.h"

#include "options.h"

namespace Vizzu
{

namespace Data
{
class DataTable;
}

namespace Gen
{

class OptionsSetter
{
public:
	explicit OptionsSetter(Options &options);

	OptionsSetter &clearSeries(const ChannelId &channelId);

	OptionsSetter &addSeries(const ChannelId &channelId,
	    const std::string &seriesName,
	    std::optional<size_t> pos = std::nullopt);

	OptionsSetter &deleteSeries(const ChannelId &channelId,
	    const std::string &seriesName);

	OptionsSetter &addSeries(const ChannelId &channelId,
	    const Data::SeriesIndex &index,
	    std::optional<size_t> pos);
	OptionsSetter &deleteSeries(const ChannelId &channelId,
	    const Data::SeriesIndex &index);
	OptionsSetter &setShape(const ShapeType &type);
	OptionsSetter &setAlign(const Base::Align::Type &alignType);
	OptionsSetter &setCoordSystem(CoordSystem coordSystem);
	OptionsSetter &setSplitted(bool value);
	OptionsSetter &rotate(double ccwQuadrant);
	OptionsSetter &setAngle(double ccwQuadrant);
	OptionsSetter &setOrientation(Options::OrientationType value);
	OptionsSetter &setFilter(const Data::Filter &filter);
	OptionsSetter &setLabelLevel(const ChannelId &channelId,
	    std::size_t level);
	OptionsSetter &setSorted(Sort value);
	OptionsSetter &setReverse(bool value);
	OptionsSetter &setRangeMin(const ChannelId &channelId,
	    const OptionalChannelExtrema &value);
	OptionsSetter &setRangeMax(const ChannelId &channelId,
	    const OptionalChannelExtrema &value);
	OptionsSetter &setStackable(const ChannelId &channelId,
	    bool value);
	OptionsSetter &setTitle(const std::optional<std::string> &title);
	OptionsSetter &setLegend(const Options::LegendType &legend);
	OptionsSetter &setAxisTitle(const ChannelId &channelId,
	    const std::string &title);
	OptionsSetter &setAxisLine(const ChannelId &channelId,
	    Base::AutoBool enable);
	OptionsSetter &setAxisLabels(const ChannelId &channelId,
	    Base::AutoBool enable);
	OptionsSetter &setTicks(const ChannelId &channelId,
	    Base::AutoBool enable);
	OptionsSetter &setGuides(const ChannelId &channelId,
	    Base::AutoBool enable);
	OptionsSetter &setMarkerGuides(const ChannelId &channelId,
	    Base::AutoBool enable);
	OptionsSetter &setInterlacing(const ChannelId &channelId,
	    Base::AutoBool enable);
	OptionsSetter &setStep(const ChannelId &channelId,
	    Base::AutoParam<double> step);
	OptionsSetter &addMarkerInfo(Options::MarkerId marker);
	OptionsSetter &moveMarkerInfo(Options::MarkerId from,
	    Options::MarkerId to);
	OptionsSetter &deleteMarkerInfo(Options::MarkerId marker);
	OptionsSetter &showTooltip(
	    std::optional<Options::MarkerId> marker);

	[[nodiscard]] const Options &getOptions() const
	{
		return options;
	}
	Options &getOptions() { return options; }
	void setTable(const Data::DataTable *table);
	[[nodiscard]] const Data::DataTable *getTable() const
	{
		return table;
	}

protected:
	Options &options;
	const Data::DataTable *table{};
};

}
}

#endif
