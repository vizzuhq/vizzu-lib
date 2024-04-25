#ifndef CHANNEL_H
#define CHANNEL_H

#include <cstdint>
#include <optional>
#include <string>

#include "base/anim/interpolated.h"
#include "data/datacube/datacube.h"
#include "data/datacube/datacubeoptions.h"
#include "data/datacube/seriesindex.h"

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

	std::pair<bool, OptionalIndex> addSeries(
	    const Data::SeriesIndex &index);
	void removeSeries(const Data::SeriesIndex &index);
	[[nodiscard]] bool isSeriesUsed(
	    const Data::SeriesIndex &index) const;
	void reset();
	[[nodiscard]] bool isEmpty() const;
	[[nodiscard]] bool isDimension() const;
	[[nodiscard]] bool isMeasure() const;
	[[nodiscard]] size_t dimensionCount() const;
	void collectDimesions(
	    Data::DataCubeOptions::IndexSet &dimensions) const;
	[[nodiscard]] std::string measureName(
	    const Data::DataCube &cube) const;
	[[nodiscard]] std::string labelDimensionName() const;
	[[nodiscard]] const DimensionIndices &dimensions() const;
	[[nodiscard]] OptionalIndex labelSeries() const;
	bool operator==(const Channel &other) const;

	Type type{};
	double defaultValue{};
	bool stackable{};
	OptionalIndex measureId{};
	DimensionIndices dimensionIds{};
	ChannelRange range{};
	std::size_t labelLevel{};
	Base::AutoParam<std::string, true> title{};
	Base::AutoBool axisLine{};
	Base::AutoBool axisLabels{};
	Base::AutoBool ticks{};
	Base::AutoBool guides{};
	Base::AutoBool markerGuides{};
	Base::AutoBool interlacing{};
	Base::AutoParam<double> step{};
};

bool isAxis(ChannelId type);

}

#endif
