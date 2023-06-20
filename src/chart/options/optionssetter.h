#ifndef OPTIONSSETTER_H
#define OPTIONSSETTER_H

#include "base/util/event.h"
#include "chart/main/style.h"

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
	typedef Util::Event<> OnFinished;
	typedef std::function<void(ChannelId, Data::SeriesIndex)>
	    OnMeasureReplaced;

	OnFinished onFinished;
	OnMeasureReplaced onMeasureReplaced;

	OptionsSetter(Options &options,
	    const OnFinished::Listener &onFinished =
	        OnFinished::Listener());
	virtual ~OptionsSetter();

	OptionsSetter &clearSeries(const ChannelId &channelId);

	OptionsSetter &addSeries(const ChannelId &channelId,
	    const std::string &seriesName,
	    std::optional<size_t> pos = std::nullopt);

	OptionsSetter &deleteSeries(const ChannelId &channelId,
	    const std::string &seriesName);

	virtual OptionsSetter &addSeries(const ChannelId &channelId,
	    const Data::SeriesIndex &index,
	    std::optional<size_t> pos = std::nullopt);
	virtual OptionsSetter &deleteSeries(const ChannelId &channelId,
	    const Data::SeriesIndex &index);
	virtual OptionsSetter &setShape(const ShapeType::Type &type);
	virtual OptionsSetter &setAlign(
	    const Base::Align::Type &alignType);
	virtual OptionsSetter &setPolar(bool value);
	virtual OptionsSetter &setSplitted(bool value);
	virtual OptionsSetter &rotate(double ccwQuadrant);
	virtual OptionsSetter &setAngle(double ccwQuadrant);
	virtual OptionsSetter &setHorizontal(bool horizontal);
	virtual OptionsSetter &setFilter(const Data::Filter &filter);
	virtual OptionsSetter &setLabelLevel(const ChannelId &channelId,
	    int level);
	virtual OptionsSetter &setSorted(bool value);
	virtual OptionsSetter &setReverse(bool value);
	virtual OptionsSetter &setRangeMin(const ChannelId &channelId,
	    const OptionalChannelExtrema &value);
	virtual OptionsSetter &setRangeMax(const ChannelId &channelId,
	    const OptionalChannelExtrema &value);
	virtual OptionsSetter &setStackable(const ChannelId &channelId,
	    bool value);
	virtual OptionsSetter &setTitle(
	    const std::optional<std::string> &title);
	virtual OptionsSetter &setLegend(const Options::Legend &legend);
	virtual OptionsSetter &setTitle(const ChannelId &channelId,
	    const std::string &title);
	virtual OptionsSetter &setAxisLine(const ChannelId &channelId,
	    Base::AutoBool enable);
	virtual OptionsSetter &setAxisLabels(const ChannelId &channelId,
	    Base::AutoBool enable);
	virtual OptionsSetter &setTicks(const ChannelId &channelId,
	    Base::AutoBool enable);
	virtual OptionsSetter &setGuides(const ChannelId &channelId,
	    Base::AutoBool enable);
	virtual OptionsSetter &setMarkerGuides(const ChannelId &channelId,
	    Base::AutoBool enable);
	virtual OptionsSetter &setInterlacing(const ChannelId &channelId,
	    Base::AutoBool enable);
	virtual OptionsSetter &setStep(const ChannelId &channelId,
	    Base::AutoParam<double> step);
	virtual void replaceOptions(const Options &options);
	virtual OptionsSetter &addMarkerInfo(Options::MarkerId marker);
	virtual OptionsSetter &moveMarkerInfo(Options::MarkerId from,
	    Options::MarkerId to);
	virtual OptionsSetter &deleteMarkerInfo(Options::MarkerId marker);
	virtual OptionsSetter &showTooltip(Options::MarkerId marker);

	const Options &getOptions() const { return options; }
	Options &getOptions() { return options; }
	void setTable(const Data::DataTable *table);
	const Data::DataTable *getTable() const { return table; }

protected:
	Options &options;
	bool changed;
	const Data::DataTable *table;
};

typedef std::shared_ptr<OptionsSetter> OptionsSetterPtr;

}
}

#endif
