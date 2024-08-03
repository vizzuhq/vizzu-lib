#include "trig.h"

#include <cmath>
#include <numbers>

int Math::rad2quadrant(double angle)
{
	return static_cast<int>(round(angle / (std::numbers::pi / 2.0)));
}
