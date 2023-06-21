
#include "shapetype.h"

using namespace Vizzu;
using namespace Vizzu::Gen;

bool Vizzu::Gen::canOverlap(ShapeType::Type type)
{
	switch (type) {
	case ShapeType::Type::rectangle:
	case ShapeType::Type::area: return false;

	case ShapeType::Type::circle:
	case ShapeType::Type::line: return true;

	default:
		throw std::logic_error("internal error: invalid shape type");
	};
}

bool Vizzu::Gen::isConnecting(ShapeType::Type type)
{
	switch (type) {
	case ShapeType::Type::rectangle:
	case ShapeType::Type::circle: return false;

	case ShapeType::Type::line:
	case ShapeType::Type::area: return true;

	default:
		throw std::logic_error("internal error: invalid shape type");
	};
}
