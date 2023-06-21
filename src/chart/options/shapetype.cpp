
#include "shapetype.h"

using namespace Vizzu;
using namespace Vizzu::Gen;

bool Vizzu::Gen::isConnecting(ShapeType type)
{
	switch (type) {
	case ShapeType::rectangle:
	case ShapeType::circle: return false;

	case ShapeType::line:
	case ShapeType::area: return true;

	default:
		throw std::logic_error("internal error: invalid shape type");
	};
}
