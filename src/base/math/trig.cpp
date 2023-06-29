#include "trig.h"

#include <cmath>

int Math::rad2quadrant(double angle)
{
	return static_cast<int>(round(angle / (M_PI / 2.0)));
}
