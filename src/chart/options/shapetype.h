#ifndef SHAPETYPE_H
#define SHAPETYPE_H

#include <array>
#include <string>

#include "base/math/fuzzybool.h"
#include "base/refl/auto_enum.h"

namespace Vizzu::Gen
{

enum class ShapeType : uint32_t { rectangle, circle, area, line };

bool isConnecting(ShapeType type);

}

#endif
