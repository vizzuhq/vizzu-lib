#include "channel.h"

#include <algorithm>
#include <cmath>
#include <set>

using namespace Vizzu;
using namespace Vizzu::Gen;

bool Vizzu::Gen::isAxis(ChannelId type)
{
	return type == Gen::ChannelId::x || type == Gen::ChannelId::y;
}

Channel::Channel() { labelLevel.set(0); }

Channel::Channel(Type type, double def, bool stackable) :
    type(type),
    defaultValue(def),
    stackable(stackable)
{
	labelLevel.set(0);
}

Channel Channel::makeChannel(Type id)
{
	switch (id) {
	case ChannelId::color: return Channel(ChannelId::color, 0, false);
	case ChannelId::label: return Channel(ChannelId::label, 0, false);
	case ChannelId::lightness:
		return Channel(ChannelId::lightness, 0.5, false);
	case ChannelId::size: return Channel(ChannelId::size, 0, true);
	case ChannelId::x: return Channel(ChannelId::x, 1, true);
	case ChannelId::y: return Channel(ChannelId::y, 1, true);
	case ChannelId::noop: return Channel(ChannelId::noop, 0, false);
	default:;
	};
	throw std::logic_error("internal error: invalid channel id");
}

std::pair<bool, Channel::OptionalIndex> Channel::addSeries(
    const Data::SeriesIndex &index,
    std::optional<size_t> pos)
{
	if (index.getType().isDimension()) {
		if (pos) {
			auto actPos = dimensionIds->getIndex(index);
			if ((int)*pos == actPos) return {false, std::nullopt};
			dimensionIds->remove(index);
			return {dimensionIds->insertAt(*pos, index),
			    std::nullopt};
		}
		else
			return {dimensionIds->pushBack(index), std::nullopt};
	}
	else {
		if (!*measureId) {
			measureId = index;
			return {true, std::nullopt};
		}
		else if (**measureId != index) {
			auto replaced = *measureId;
			measureId = index;
			return {true, replaced};
		}
		else
			return {false, std::nullopt};
	}
}

bool Channel::removeSeries(const Data::SeriesIndex &index)
{
	if (index.getType().isDimension()) {
		return dimensionIds->remove(index);
	}
	else {
		if (*measureId) {
			measureId = std::nullopt;
			return true;
		}
		else
			return false;
	}
}

bool Channel::isSeriesUsed(const Data::SeriesIndex &index) const
{
	return (measureId() && *(measureId()) == index)
	    || (dimensionIds().includes(index));
}

int Channel::findPos(const Data::SeriesIndex &index) const
{
	if (index.getType().isMeasure())
		return (measureId() && *(measureId()) == index) ? 0 : -1;
	else
		return dimensionIds().getIndex(index);
}

void Channel::reset()
{
	measureId = std::nullopt;
	dimensionIds->clear();
	title.set("auto");
	axisLine.set(Base::AutoBool());
	axisLabels.set(Base::AutoBool());
	ticks.set(Base::AutoBool());
	interlacing.set(Base::AutoBool());
	guides.set(Base::AutoBool());
	markerGuides.set(Base::AutoBool());
	labelLevel.set(0);
}

void Channel::clearMeasure() { measureId = std::nullopt; }

bool Channel::isEmpty() const
{
	return (!measureId.data && dimensionIds.data.empty());
}

bool Channel::isDimension() const
{
	return !measureId();
}

bool Channel::isMeasure() const
{
	return !isEmpty() && measureId();
}

size_t Channel::dimensionCount() const { return dimensionIds().size(); }

void Channel::collectDimesions(
    Data::DataCubeOptions::IndexSet &dimensions) const
{
	for (const auto &dimension : dimensionIds())
		dimensions.insert(dimension);
}

void Channel::collectRealSeries(
    Data::DataCubeOptions::IndexSet &series) const
{
	if (measureId()) series.insert(*measureId());
}

bool Channel::operator==(const Channel &other) const
{
	return type() == other.type()
	    && measureId() == other.measureId()
	    && dimensionIds() == other.dimensionIds()
	    && (defaultValue() == other.defaultValue()
	        || (std::isnan(defaultValue())
	            && std::isnan(other.defaultValue())))
	    && stackable() == other.stackable()
	    && range.get() == other.range.get()
	    && labelLevel.get() == other.labelLevel.get()
	    && title.get() == other.title.get()
	    && axisLine.get() == other.axisLine.get()
	    && axisLabels.get() == other.axisLabels.get()
	    && ticks.get() == other.ticks.get()
	    && interlacing.get() == other.interlacing.get()
	    && guides.get() == other.guides.get()
	    && markerGuides.get() == other.markerGuides.get();
}

std::string Channel::measureName(const Data::DataTable &table) const
{
	if (!isEmpty() && measureId() && !isDimension()) {
		return measureId()->toString(table);
	}
	else
		return std::string();
}

std::list<std::string> Channel::dimensionNames(
    const Data::DataTable &table) const
{
	std::list<std::string> res;
	for (auto &dimensionId : dimensionIds())
		res.push_back(dimensionId.toString(table));
	return res;
}

Channel::DimensionIndices Vizzu::Gen::operator&(
    const Channel::DimensionIndices &x,
    const Channel::DimensionIndices &y)
{
	std::set<Data::SeriesIndex> merged;
	for (const auto &id : x) merged.insert(id);
	for (const auto &id : y) merged.insert(id);

	Channel::DimensionIndices res;
	for (const auto &id : merged) res.pushBack(id);
	return res;
}

Channel::OptionalIndex Channel::labelSeries() const
{
	if (isDimension()) {
		auto level = floor(labelLevel.get());
		if (level >= 0 && level < dimensionIds().size())
			return dimensionIds().at(level);
		else
			return std::nullopt;
	}
	else
		return measureId();
}
void Channel::setStackable(bool value) {
	stackable = value;
}
