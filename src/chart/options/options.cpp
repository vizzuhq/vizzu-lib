
#include "options.h"

#include "base/math/trig.h"

using namespace Vizzu;
using namespace Vizzu::Diag;

Options::Options()
{
	alignType.set(Base::Align::None);
	polar.set(false);
	angle.set(0.0);
	horizontal.set(true);
	sorted.set(false);
	reverse.set(false);
	bubbleChartAlgorithm.set(BubbleChartAlgorithm::slow);
}

void Options::reset()
{
	scales.reset();
	title.set(title.get().get().has_value() 
		? Title(std::string()) : Title());
}

const Scale *Options::subAxisOf(Scales::Id id) const
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
				|| (id.type == ScaleId::size && scales.anyAxisSet())
			? &scales.at(Scales::Id{ ScaleId::size, id.index })
			: nullptr;

	case ShapeType::Type::Circle:
		// todo: should return 2 scale (size + other axis)
		if (id.type == ScaleId::size && scales.anyAxisSet()) {
			return &scales.at(Scales::Id{ ScaleId::size, id.index });
		} else if (isAxis(id.type)) {
			if (scales.at(id).isPseudoDiscrete() && id.type == mainAxisType())
				return  &subAxis(id.index);
			else
				return &scales.at(Scales::Id{ ScaleId::size, id.index });
		} else return nullptr;

	default:
		return nullptr;
	}
}

ScaleId Options::stackAxisType() const
{
	switch ((ShapeType::Type)shapeType.get())
	{
	case ShapeType::Type::Area:
	case ShapeType::Type::Rectangle: return subAxisType();

	default:
	case ShapeType::Type::Circle:
	case ShapeType::Type::Line: return ScaleId::size;
	}
}

bool Options::operator==(const Options &other) const
{
	return polar.get() == other.polar.get()
	        && angle.get() == other.angle.get()
	        && shapeType.get() == other.shapeType.get()
			&& horizontal.get() == other.horizontal.get()
			&& splitted.get() == other.splitted.get()
			&& dataFilter.get() == other.dataFilter.get()
			&& alignType.get() == other.alignType.get()
			&& splitted.get() == other.splitted.get()
			&& scales == other.scales
	        && sorted.get() == other.sorted.get()
	        && reverse.get() == other.reverse.get()
			&& title.get() == other.title.get()
			&& legend.get() == other.legend.get();
}

ScaleId Options::getHorizontalScale() const
{
	return (Math::rad2quadrant(angle.get()) % 2) == 0
			? ScaleId::x : ScaleId::y;
}

ScaleId Options::getVeritalScale() const
{
	return getHorizontalScale() == ScaleId::x
			? ScaleId::y : ScaleId::x;
}

bool Options::isShapeValid(const ShapeType::Type &shapeType) const
{
	if (scales.anyAxisSet()
		&& mainAxis().discreteCount() > 0)
	{
		return true;
	}
	else return shapeType == ShapeType::Rectangle
				|| shapeType == ShapeType::Circle;
}
