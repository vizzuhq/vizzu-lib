
#include "diagramoptions.h"

#include "base/math/trig.h"

using namespace Vizzu;
using namespace Vizzu::Diag;

DiagramOptions::DiagramOptions()
{
	alignType.set(Base::Align::None);
	polar.set(false);
	angle.set(0.0);
	horizontal.set(true);
	sorted.set(false);
	reverse.set(false);
	bubbleChartAlgorithm.set(BubbleChartAlgorithm::slow);
}

const Scale *DiagramOptions::subAxisOf(Scales::Id id) const
{
	switch ((ShapeType::Type)shapeType.get())
	{
	case ShapeType::Type::Rectangle:
		return id.type == mainAxisType()
				? &subAxis(id.index)
				: nullptr;

	case ShapeType::Type::Area:
		return	id.type == mainAxisType() ? &subAxis(id.index) :
				id.type == subAxisType() ? &mainAxis(id.index) :
				nullptr;

	case ShapeType::Type::Line:
		return id.type == subAxisType()
				|| (id.type == Scale::Type::Size && scales.anyAxisSet())
			? &scales.at(Scales::Id{ Scale::Type::Size, id.index })
			: nullptr;

	case ShapeType::Type::Circle:
		// todo: should return 2 scale (size + other axis)
		if (id.type == Scale::Type::Size && scales.anyAxisSet()) {
			return &scales.at(Scales::Id{ Scale::Type::Size, id.index });
		} else if (isAxis(id.type)) {
			if (scales.at(id).isPseudoDiscrete() && id.type == mainAxisType())
				return  &subAxis(id.index);
			else
				return &scales.at(Scales::Id{ Scale::Type::Size, id.index });
		} else return nullptr;

	default:
		return nullptr;
	}
}

Scale::Type DiagramOptions::stackAxisType() const
{
	switch ((ShapeType::Type)shapeType.get())
	{
	case ShapeType::Type::Area:
	case ShapeType::Type::Rectangle: return subAxisType();

	default:
	case ShapeType::Type::Circle:
	case ShapeType::Type::Line: return Scale::Type::Size;
	}
}

bool DiagramOptions::operator==(const DiagramOptions &other) const
{
	return polar.get() == other.polar.get()
	        && angle.get() == other.angle.get()
	        && shapeType.get() == other.shapeType.get()
			&& horizontal.get() == other.horizontal.get()
			&& splitted.get() == other.splitted.get()
			&& dataFilters.get() == other.dataFilters.get()
			&& alignType.get() == other.alignType.get()
			&& splitted.get() == other.splitted.get()
			&& scales == other.scales
	        && sorted.get() == other.sorted.get()
	        && reverse.get() == other.reverse.get()
			&& title.get() == other.title.get()
			&& legend.get() == other.legend.get();
}

Scale::Type DiagramOptions::getHorizontalScale() const
{
	return (Math::rad2quadrant(angle.get()) % 2) == 0
			? Scale::Type::X : Scale::Type::Y;
}

Scale::Type DiagramOptions::getVeritalScale() const
{
	return getHorizontalScale() == Scale::Type::X
			? Scale::Type::Y : Scale::Type::X;
}

bool DiagramOptions::isShapeValid(const ShapeType::Type &shapeType) const
{
	if (scales.anyAxisSet()
		&& mainAxis().discreteCount() > 0)
	{
		return true;
	}
	else return shapeType == ShapeType::Rectangle
				|| shapeType == ShapeType::Circle;
}
