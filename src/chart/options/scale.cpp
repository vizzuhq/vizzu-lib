#include "scale.h"

#include <algorithm>
#include <cmath>
#include <set>

using namespace Vizzu;
using namespace Vizzu::Diag;

bool Vizzu::Diag::isAxis(ScaleId type)
{
	return type == Diag::ScaleId::x
		|| type == Diag::ScaleId::y;
}

Scale::Scale()
{
	labelLevel.set(0);
}

Scale::Scale(Type type, double def, bool stackable) :
    type(type),
    defaultValue(def),
    stackable(stackable)
{
	labelLevel.set(0);
}

Scale Scale::makeScale(Type id)
{
	switch (id) {
	case ScaleId::color: return Scale(ScaleId::color, 0, false);
	case ScaleId::label: return Scale(ScaleId::label, 0, false);
	case ScaleId::lightness: return Scale(ScaleId::lightness, 0.5, false);
	case ScaleId::size: return Scale(ScaleId::size, 0, true);
	case ScaleId::x: return Scale(ScaleId::x, 1, true);
	case ScaleId::y: return Scale(ScaleId::y, 1, true);
	case ScaleId::noop: return Scale(ScaleId::noop, 0, false);
	default:;
	};
	throw std::logic_error("internal error: invalid scale id");
}

std::pair<bool, Scale::OptionalIndex>
Scale::addSeries(const Data::SeriesIndex &index, std::optional<size_t> pos)
{
	if (index.getType().isDiscrete())
	{
		if (pos) {
			auto actPos = discretesIds->getIndex(index);
			if ((int)*pos == actPos) return { false, std::nullopt };
			discretesIds->remove(index);
			return { discretesIds->insertAt(*pos, index), std::nullopt };
		} else
			return { discretesIds->pushBack(index), std::nullopt };
	}
	else
	{
		if (!*continousId) {
			continousId = index;
			return { true, std::nullopt };
		}
		else if (**continousId != index) {
			auto replaced = *continousId;
			continousId = index;
			return { true, replaced };
		}
		else return { false, std::nullopt };
	}
}

bool Scale::removeSeries(const Data::SeriesIndex &index)
{
	if (index.getType().isDiscrete())
	{
		return discretesIds->remove(index);
	}
	else
	{
		if (*continousId) {
			continousId = std::nullopt;
			return true;
		} else return false;
	}
}

bool Scale::isSeriesUsed(const Data::SeriesIndex &index) const
{
	return (continousId() && *(continousId()) == index)
		|| (discretesIds().includes(index));
}

int Scale::findPos(const Data::SeriesIndex &index) const
{
	if (index.getType().isContinous())
		return (continousId() && *(continousId()) == index) ? 0 : -1;
	else
		return discretesIds().getIndex(index);
}

void Scale::reset()
{
	continousId = std::nullopt;
	discretesIds->clear();
	title.set("auto");
	labelLevel.set(0);
}

bool Scale::isEmpty() const
{
	return (!continousId.data && discretesIds.data.empty());
}

bool Scale::isPseudoDiscrete() const
{
	return !continousId();
}

size_t Scale::discreteCount() const
{
	return discretesIds().size();
}

void Scale::collectDimesions(Data::DataCubeOptions::IndexSet &dimensions) const
{
	for (const auto &discrete : discretesIds())
		dimensions.insert(discrete);
}

void Scale::collectRealSeries(Data::DataCubeOptions::IndexSet &series) const
{
	if (!isPseudoDiscrete())
		series.insert(*continousId());
}

bool Scale::operator==(const Scale &other) const
{
	return type() == other.type()
			&& continousId() == other.continousId()
			&& discretesIds() == other.discretesIds()
			&& (defaultValue() == other.defaultValue()
					|| (std::isnan(defaultValue()) && std::isnan(other.defaultValue())))
			&& stackable() == other.stackable()
			&& range.get() == other.range.get()
			&& labelLevel.get() == other.labelLevel.get()
			&& title.get() == other.title.get();
}

std::string Scale::continousName(const Data::DataTable &table) const
{
	if (!isEmpty() && continousId() && !isPseudoDiscrete())
	{
		return continousId()->toString(table);
	}
	else return std::string();
}

std::list<std::string> Scale::discreteNames(const Data::DataTable &table) const
{
	std::list<std::string> res;
	for (auto &discreteId: discretesIds())
		res.push_back(discreteId.toString(table));
	return res;
}

Scale::DiscreteIndices
Vizzu::Diag::operator&(const Scale::DiscreteIndices &x,
						 const Scale::DiscreteIndices &y)
{
	std::set<Data::SeriesIndex> merged;
	for (const auto &id : x) merged.insert(id);
	for (const auto &id : y) merged.insert(id);

	Scale::DiscreteIndices res;
	for (const auto &id : merged) res.pushBack(id);
	return res;
}

Scale::OptionalIndex Scale::labelSeries() const
{
	if (isPseudoDiscrete()) 
	{
		auto level = floor(labelLevel.get());
		if (level >= 0 && level < discretesIds().size()) 
			return discretesIds().at(level);
		else return std::nullopt;
	}
	else return continousId();
}