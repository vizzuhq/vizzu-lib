#ifndef OPTIONSSETTER_H
#define OPTIONSSETTER_H

#include "base/util/event.h"

#include "options.h"

namespace Vizzu::Gen
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
	OptionsSetter &setFilter(const Data::Filter &filter);
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

#endif
