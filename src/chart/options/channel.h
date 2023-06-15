#ifndef CHANNEL_H
#define CHANNEL_H

#include <cstdint>
#include <istream>
#include <list>
#include <optional>
#include <string>

#include "base/anim/interpolated.h"
#include "base/refl/enum.h"
#include "base/util/templates.h"
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

class Channel : Util::AddReadOnly<Channel>
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
	bool isPseudoDimension() const;
	bool isMeasure() const;
	size_t dimensionCount() const;
	int findPos(const Data::SeriesIndex &index) const;
	void collectDimesions(
	    Data::DataCubeOptions::IndexSet &dimensions) const;
	void collectRealSeries(
	    Data::DataCubeOptions::IndexSet &series) const;
	std::string measureName(const Data::DataTable &table) const;
	std::list<std::string> dimensionNames(
	    const Data::DataTable &table) const;
	OptionalIndex labelSeries() const;
	bool operator==(const Channel &other) const;

	ReadOnly<Type> type;
	ReadOnly<OptionalIndex> measureId;
	ReadOnly<DimensionIndices> dimensionIds;
	ReadOnly<double> defaultValue;
	ReadOnly<bool> stackable;
	Util::ReadWrite<ChannelRange> range;
	Util::ReadWrite<double> labelLevel;
	Util::ReadWrite<std::string> title;
	Util::ReadWrite<Base::AutoBool> axisLine;
	Util::ReadWrite<Base::AutoBool> axisLabels;
	Util::ReadWrite<Base::AutoBool> ticks;
	Util::ReadWrite<Base::AutoBool> guides;
	Util::ReadWrite<Base::AutoBool> markerGuides;
	Util::ReadWrite<Base::AutoBool> interlacing;
	Util::ReadWrite<Base::AutoParam<double>> step;
};

Channel::DimensionIndices operator&(const Channel::DimensionIndices &x,
    const Channel::DimensionIndices &y);

bool isAxis(ChannelId type);

}
}

#endif
