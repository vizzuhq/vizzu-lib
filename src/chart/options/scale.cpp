#include "scale.h"

#include <algorithm>
#include <cmath>
#include <set>

using namespace Vizzu;
using namespace Vizzu::Diag;

std::string Vizzu::Diag::toString(Scale::Type type)
{
	switch (type) {
	case Scale::Color: return "Color";
	case Scale::Lightness: return "Lightness";
	case Scale::Size: return "Size";
	case Scale::Shape: return "Shape";
	case Scale::Label: return "Label";
	case Scale::Timeline: return "Timeline";
	case Scale::X: return "X";
	case Scale::Y: return "Y";
	default:
	case Scale::id_size: throw std::logic_error("invalid scale id");
	}
}

Scale::Type Vizzu::Diag::toScaleType(const std::string &type)
{
	if(type == "color") return Scale::Color;
	if(type == "lightness") return Scale::Lightness;
	if(type == "size") return Scale::Size;
	if(type == "shape") return Scale::Shape;
	if(type == "label") return Scale::Label;
	if(type == "timeline") return Scale::Timeline;
	if(type == "y") return Scale::Y;
	if(type == "x") return Scale::X;
	throw std::logic_error("invalid scale name");
}

bool Vizzu::Diag::isAxis(Scale::Type type)
{
	return type == Diag::Scale::X
		|| type == Diag::Scale::Y;
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
	case Color: return Scale(Color, 0, false);
	case Shape: return Scale(Shape, 0, false);
	case Label: return Scale(Label, 0, false);
	case Timeline: return Scale(Timeline, 0, true);
	case Lightness: return Scale(Lightness, 0.5, false);
	case Size: return Scale(Size, 0, true);
	case X: return Scale(X, 1, true);
	case Y: return Scale(Y, 1, true);
	default:;
	};
	throw std::logic_error("internal error: invalid scale id");
}

std::pair<bool, Scale::OptionalContinousIndex>
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
	labelLevel.set(0);
}

bool Scale::isEmpty() const
{
	return (!continousId.data && discretesIds.data.empty());
}

bool Scale::isPseudoDiscrete() const
{
	return !continousId()
			|| continousId()->getType() == Data::SeriesType::Exists;
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
