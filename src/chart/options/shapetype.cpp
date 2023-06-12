
#include "shapetype.h"

using namespace Vizzu;
using namespace Vizzu::Diag;

std::string Vizzu::Diag::toString(ShapeType::Type type)
{
	switch (type) {
	case ShapeType::Rectangle: return "Rectangle";
	case ShapeType::Circle: return "Circle";
	case ShapeType::Area: return "Area";
	case ShapeType::Line: return "Line";
	default:
	case ShapeType::size:
		throw std::logic_error("invalid shape type");
	};
}

bool Vizzu::Diag::canOverlap(ShapeType::Type type)
{
	switch (type) {
	case ShapeType::Rectangle:
	case ShapeType::Area: return false;

	case ShapeType::Circle:
	case ShapeType::Line: return true;

	default:
	case ShapeType::size:
		throw std::logic_error("internal error: invalid shape type");
	};
}

bool Vizzu::Diag::isConnecting(ShapeType::Type type)
{
	switch (type) {
	case ShapeType::Rectangle:
	case ShapeType::Circle: return false;

	case ShapeType::Line:
	case ShapeType::Area: return true;

	default:
	case ShapeType::size:
		throw std::logic_error("internal error: invalid shape type");
	};
}
