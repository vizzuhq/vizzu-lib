#include "scale.h"

#include <algorithm>
#include <cmath>
#include <set>

using namespace Vizzu;
using namespace Vizzu::Diag;

bool Vizzu::Diag::isAxis(ScaleId type)
{
	return type == Diag::ScaleId::x || type == Diag::ScaleId::y;
}

Scale::Scale() { labelLevel.set(0); }

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
	case ScaleId::lightness:
		return Scale(ScaleId::lightness, 0.5, false);
	case ScaleId::size: return Scale(ScaleId::size, 0, true);
	case ScaleId::x: return Scale(ScaleId::x, 1, true);
	case ScaleId::y: return Scale(ScaleId::y, 1, true);
	case ScaleId::noop: return Scale(ScaleId::noop, 0, false);
	default:;
	};
	throw std::logic_error("internal error: invalid scale id");
}

std::pair<bool, Scale::OptionalIndex> Scale::addSeries(
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

bool Scale::removeSeries(const Data::SeriesIndex &index)
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

bool Scale::isSeriesUsed(const Data::SeriesIndex &index) const
{
	return (measureId() && *(measureId()) == index)
	    || (dimensionIds().includes(index));
}

int Scale::findPos(const Data::SeriesIndex &index) const
{
	if (index.getType().isMeasure())
		return (measureId() && *(measureId()) == index) ? 0 : -1;
	else
		return dimensionIds().getIndex(index);
}

void Scale::reset()
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

void Scale::clearMeasure() { measureId = std::nullopt; }

bool Scale::isEmpty() const
{
	return (!measureId.data && dimensionIds.data.empty());
}

bool Scale::isPseudoDimension() const
{
	return !measureId()
	    || measureId()->getType() == Data::SeriesType::Exists;
}

bool Scale::isMeasure() const
{
	return !isEmpty() && !isPseudoDimension();
}

size_t Scale::dimensionCount() const { return dimensionIds().size(); }

void Scale::collectDimesions(
    Data::DataCubeOptions::IndexSet &dimensions) const
{
	for (const auto &dimension : dimensionIds())
		dimensions.insert(dimension);
}

void Scale::collectRealSeries(
    Data::DataCubeOptions::IndexSet &series) const
{
	if (!isPseudoDimension()) series.insert(*measureId());
}

bool Scale::operator==(const Scale &other) const
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

std::string Scale::measureName(const Data::DataTable &table) const
{
	if (!isEmpty() && measureId() && !isPseudoDimension()) {
		return measureId()->toString(table);
	}
	else
		return std::string();
}

std::list<std::string> Scale::dimensionNames(
    const Data::DataTable &table) const
{
	std::list<std::string> res;
	for (auto &dimensionId : dimensionIds())
		res.push_back(dimensionId.toString(table));
	return res;
}

Scale::DimensionIndices Vizzu::Diag::operator&(
    const Scale::DimensionIndices &x,
    const Scale::DimensionIndices &y)
{
	std::set<Data::SeriesIndex> merged;
	for (const auto &id : x) merged.insert(id);
	for (const auto &id : y) merged.insert(id);

	Scale::DimensionIndices res;
	for (const auto &id : merged) res.pushBack(id);
	return res;
}

Scale::OptionalIndex Scale::labelSeries() const
{
	if (isPseudoDimension()) {
		auto level = floor(labelLevel.get());
		if (level >= 0 && level < dimensionIds().size())
			return dimensionIds().at(level);
		else
			return std::nullopt;
	}
	else
		return measureId();
}