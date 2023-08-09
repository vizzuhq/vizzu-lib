#ifndef SHAPETYPE_H
#define SHAPETYPE_H

#include <cstdint>

namespace Vizzu::Gen
{

enum class ShapeType : uint32_t { rectangle, circle, area, line };

bool isConnecting(ShapeType type);

}

#endif
