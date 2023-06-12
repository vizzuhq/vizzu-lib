#include "channel.h"

#include <algorithm>
#include <cmath>
#include <set>

using namespace Vizzu;
using namespace Vizzu::Diag;

bool Vizzu::Diag::isAxis(ChannelId type)
{
	return type == Diag::ChannelId::x || type == Diag::ChannelId::y;
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
	if (index.getType().isDiscrete()) {
		if (pos) {
			auto actPos = discretesIds->getIndex(index);
			if ((int)*pos == actPos) return {false, std::nullopt};
			discretesIds->remove(index);
			return {discretesIds->insertAt(*pos, index),
			    std::nullopt};
		}
		else
			return {discretesIds->pushBack(index), std::nullopt};
	}
	else {
		if (!*continousId) {
			continousId = index;
			return {true, std::nullopt};
		}
		else if (**continousId != index) {
			auto replaced = *continousId;
			continousId = index;
			return {true, replaced};
		}
		else
			return {false, std::nullopt};
	}
}

bool Channel::removeSeries(const Data::SeriesIndex &index)
{
	if (index.getType().isDiscrete()) {
		return discretesIds->remove(index);
	}
	else {
		if (*continousId) {
			continousId = std::nullopt;
			return true;
		}
		else
			return false;
	}
}

bool Channel::isSeriesUsed(const Data::SeriesIndex &index) const
{
	return (continousId() && *(continousId()) == index)
	    || (discretesIds().includes(index));
}

int Channel::findPos(const Data::SeriesIndex &index) const
{
	if (index.getType().isContinous())
		return (continousId() && *(continousId()) == index) ? 0 : -1;
	else
		return discretesIds().getIndex(index);
}

void Channel::reset()
{
	continousId = std::nullopt;
	discretesIds->clear();
	title.set("auto");
	axisLine.set(Base::AutoBool());
	axisLabels.set(Base::AutoBool());
	ticks.set(Base::AutoBool());
	interlacing.set(Base::AutoBool());
	guides.set(Base::AutoBool());
	markerGuides.set(Base::AutoBool());
	labelLevel.set(0);
}

void Channel::clearContinuous() { continousId = std::nullopt; }

bool Channel::isEmpty() const
{
	return (!continousId.data && discretesIds.data.empty());
}

bool Channel::isPseudoDiscrete() const
{
	return !continousId()
	    || continousId()->getType() == Data::SeriesType::Exists;
}

bool Channel::isContinuous() const
{
	return !isEmpty() && !isPseudoDiscrete();
}

size_t Channel::discreteCount() const { return discretesIds().size(); }

void Channel::collectDimesions(
    Data::DataCubeOptions::IndexSet &dimensions) const
{
	for (const auto &discrete : discretesIds())
		dimensions.insert(discrete);
}

void Channel::collectRealSeries(
    Data::DataCubeOptions::IndexSet &series) const
{
	if (!isPseudoDiscrete()) series.insert(*continousId());
}

bool Channel::operator==(const Channel &other) const
{
	return type() == other.type()
	    && continousId() == other.continousId()
	    && discretesIds() == other.discretesIds()
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

std::string Channel::continousName(const Data::DataTable &table) const
{
	if (!isEmpty() && continousId() && !isPseudoDiscrete()) {
		return continousId()->toString(table);
	}
	else
		return std::string();
}

std::list<std::string> Channel::discreteNames(
    const Data::DataTable &table) const
{
	std::list<std::string> res;
	for (auto &discreteId : discretesIds())
		res.push_back(discreteId.toString(table));
	return res;
}

Channel::DiscreteIndices Vizzu::Diag::operator&(
    const Channel::DiscreteIndices &x,
    const Channel::DiscreteIndices &y)
{
	std::set<Data::SeriesIndex> merged;
	for (const auto &id : x) merged.insert(id);
	for (const auto &id : y) merged.insert(id);

	Channel::DiscreteIndices res;
	for (const auto &id : merged) res.pushBack(id);
	return res;
}

Channel::OptionalIndex Channel::labelSeries() const
{
	if (isPseudoDiscrete()) {
		auto level = floor(labelLevel.get());
		if (level >= 0 && level < discretesIds().size())
			return discretesIds().at(level);
		else
			return std::nullopt;
	}
	else
		return continousId();
}