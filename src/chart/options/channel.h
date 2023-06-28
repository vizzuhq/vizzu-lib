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

namespace Vizzu
{
namespace Gen
{

enum class ChannelId { color, lightness, size, label, x, y, noop };

class Channel
{
public:
	typedef ChannelId Type;

	typedef std::optional<Data::SeriesIndex> OptionalIndex;
	typedef Data::SeriesList DimensionIndices;

	static Channel makeChannel(Type id);

	Channel();
	Channel(const Channel &channel) = default;
	Channel(Channel &&) = default;
	Channel &operator=(const Channel &) = default;
	Channel &operator=(Channel &&) = default;

	Channel(Type type, double def, bool stackable);
	std::pair<bool, OptionalIndex> addSeries(
	    const Data::SeriesIndex &index,
	    std::optional<size_t> pos = std::nullopt);
	bool removeSeries(const Data::SeriesIndex &index);
	bool isSeriesUsed(const Data::SeriesIndex &index) const;
	void reset();
	void clearMeasure();
	bool isEmpty() const;
	bool isDimension() const;
	bool isMeasure() const;
	size_t dimensionCount() const;
	std::optional<int> findPos(const Data::SeriesIndex &index) const;
	void collectDimesions(
	    Data::DataCubeOptions::IndexSet &dimensions) const;
	void collectRealSeries(
	    Data::DataCubeOptions::IndexSet &series) const;
	std::string measureName(const Data::DataTable &table) const;
	std::list<std::string> dimensionNames(
	    const Data::DataTable &table) const;
	OptionalIndex labelSeries() const;
	bool operator==(const Channel &other) const;

	Type type;
	OptionalIndex measureId;
	DimensionIndices dimensionIds;
	double defaultValue;
	bool stackable;
	ChannelRange range;
	double labelLevel;
	std::string title;
	Base::AutoBool axisLine;
	Base::AutoBool axisLabels;
	Base::AutoBool ticks;
	Base::AutoBool guides;
	Base::AutoBool markerGuides;
	Base::AutoBool interlacing;
	Base::AutoParam<double> step;
};

Channel::DimensionIndices operator&(const Channel::DimensionIndices &x,
    const Channel::DimensionIndices &y);

bool isAxis(ChannelId type);

}
}

#endif
