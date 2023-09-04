#ifndef CHANNEL_H
#define CHANNEL_H

#include <cstdint>
#include <istream>
#include <list>
#include <optional>
#include <string>

#include "base/anim/interpolated.h"
#include "data/datacube/datacubeoptions.h"
#include "data/datacube/seriesindex.h"
#include "data/table/datatable.h"

#include "autoparam.h"
#include "channelrange.h"

namespace Vizzu::Gen
{

enum class ChannelId { color, lightness, size, label, x, y, noop };

class Channel
{
public:
	using Type = ChannelId;

	using OptionalIndex = std::optional<Data::SeriesIndex>;
	using DimensionIndices = Data::SeriesList;

	static Channel makeChannel(Type id);

	Channel(const Channel &channel) = default;
	Channel(Channel &&) = default;
	Channel &operator=(const Channel &) = default;
	Channel &operator=(Channel &&) = default;

	Channel(Type type, double def, bool stackable);
	std::pair<bool, OptionalIndex> addSeries(
	    const Data::SeriesIndex &index,
	    std::optional<size_t> pos = std::nullopt);
	bool removeSeries(const Data::SeriesIndex &index);
	[[nodiscard]] bool isSeriesUsed(
	    const Data::SeriesIndex &index) const;
	void reset();
	void clearMeasure();
	[[nodiscard]] bool isEmpty() const;
	[[nodiscard]] bool isDimension() const;
	[[nodiscard]] bool isMeasure() const;
	[[nodiscard]] size_t dimensionCount() const;
	void collectDimesions(
	    Data::DataCubeOptions::IndexSet &dimensions) const;
	void collectRealSeries(
	    Data::DataCubeOptions::IndexSet &series) const;
	[[nodiscard]] std::string measureName(
	    const Data::DataTable &table) const;
	[[nodiscard]] std::list<std::string> dimensionNames(
	    const Data::DataTable &table) const;
	[[nodiscard]] OptionalIndex labelSeries() const;
	bool operator==(const Channel &other) const;

	Type type;
	OptionalIndex measureId;
	DimensionIndices dimensionIds;
	double defaultValue;
	bool stackable;
	ChannelRange range;
	double labelLevel{0.0};
	std::string title = "auto";
	Base::AutoBool axisLine;
	Base::AutoBool axisLabels;
	Base::AutoBool ticks;
	Base::AutoBool guides;
	Base::AutoBool markerGuides;
	Base::AutoBool interlacing;
	Base::AutoParam<double> step;
};

Channel::DimensionIndices operator&(
    const Channel::DimensionIndices &x,
    const Channel::DimensionIndices &y);

bool isAxis(ChannelId type);

}

#endif
