#include "trig.h"

#include <cmath>

int Math::rad2quadrant(double angle)
{
	return (int)round(angle / (M_PI / 2.0));
}
