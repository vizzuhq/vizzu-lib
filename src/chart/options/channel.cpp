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

Channel::Channel() { labelLevel = 0; }

Channel::Channel(Type type, double def, bool stackable) :
    type(type),
    defaultValue(def),
    stackable(stackable)
{
	labelLevel = 0;
}

Channel Channel::makeChannel(Type id)
{
	switch (id) {
	case ChannelId::color: return {ChannelId::color, 0, false};
	case ChannelId::label: return {ChannelId::label, 0, false};
	case ChannelId::lightness:
		return {ChannelId::lightness, 0.5, false};
	case ChannelId::size: return {ChannelId::size, 0, true};
	case ChannelId::x: return {ChannelId::x, 1, true};
	case ChannelId::y: return {ChannelId::y, 1, true};
	case ChannelId::noop: return {ChannelId::noop, 0, false};
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
			auto actPos = dimensionIds.getIndex(index);
			if (static_cast<int>(*pos) == actPos) return {false, std::nullopt};
			dimensionIds.remove(index);
			return {dimensionIds.insertAt(*pos, index),
			    std::nullopt};
		}

		return {dimensionIds.pushBack(index), std::nullopt};
	}
	if (!measureId) {
		measureId = index;
		return {true, std::nullopt};
	}
	if (*measureId != index) {
		auto replaced = *measureId;
		measureId = index;
		return {true, replaced};
	}
	return {false, std::nullopt};
}

bool Channel::removeSeries(const Data::SeriesIndex &index)
{
	if (index.getType().isDimension()) {
		return dimensionIds.remove(index);
	}
	if (measureId) {
		measureId = std::nullopt;
		return true;
	}

	return false;
}

bool Channel::isSeriesUsed(const Data::SeriesIndex &index) const
{
	return (measureId && *measureId == index)
	    || (dimensionIds.includes(index));
}

void Channel::reset()
{
	measureId = std::nullopt;
	dimensionIds.clear();
	title = "auto";
	axisLine = Base::AutoBool();
	axisLabels = Base::AutoBool();
	ticks = Base::AutoBool();
	interlacing = Base::AutoBool();
	guides = Base::AutoBool();
	markerGuides = Base::AutoBool();
	labelLevel = 0;
}

void Channel::clearMeasure() { measureId = std::nullopt; }

bool Channel::isEmpty() const
{
	return (!measureId && dimensionIds.empty());
}

bool Channel::isDimension() const
{
	return !measureId;
}

bool Channel::isMeasure() const
{
	return !isEmpty() && measureId;
}

size_t Channel::dimensionCount() const { return dimensionIds.size(); }

void Channel::collectDimesions(
    Data::DataCubeOptions::IndexSet &dimensions) const
{
	for (const auto &dimension : dimensionIds)
		dimensions.insert(dimension);
}

void Channel::collectRealSeries(
    Data::DataCubeOptions::IndexSet &series) const
{
	if (measureId) series.insert(*measureId);
}

bool Channel::operator==(const Channel &other) const
{
	return type == other.type
	    && measureId == other.measureId
	    && dimensionIds == other.dimensionIds
	    && (defaultValue == other.defaultValue
	        || (std::isnan(defaultValue)
	            && std::isnan(other.defaultValue)))
	    && stackable == other.stackable
	    && range == other.range
	    && labelLevel == other.labelLevel
	    && title == other.title
	    && axisLine == other.axisLine
	    && axisLabels == other.axisLabels
	    && ticks == other.ticks
	    && interlacing == other.interlacing
	    && guides == other.guides
	    && markerGuides == other.markerGuides;
}

std::string Channel::measureName(const Data::DataTable &table) const
{
	if (!isEmpty() && measureId && !isDimension()) {
		return measureId->toString(table);
	}
	return {};
}

std::list<std::string> Channel::dimensionNames(
    const Data::DataTable &table) const
{
	std::list<std::string> res;
	for (const auto &dimensionId : dimensionIds)
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
		auto level = floor(labelLevel);
		if (level >= 0 && level < dimensionIds.size())
			return dimensionIds.at(level);
		return std::nullopt;
	}
	return measureId;
}
