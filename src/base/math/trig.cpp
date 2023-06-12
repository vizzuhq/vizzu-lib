#include "trig.h"

#include <array>
#include <cmath>

float Math::atan2(float y, float x)
{
	const auto ONEQTR_PI = (float)(M_PI / 4.0);
	const auto THRQTR_PI = (float)(3.0 * M_PI / 4.0);
	float r;
	float angle;
	float abs_y = fabs(y) + 1e-10f; // kludge to prevent 0/0 condition
	if (x < 0.0f) {
		r = (x + abs_y) / (abs_y - x);
		angle = THRQTR_PI;
	}
	else {
		r = (x - abs_y) / (x + abs_y);
		angle = ONEQTR_PI;
	}
	angle += (0.1963f * r * r - 0.9817f) * r;
	if (y < 0.0f)
		return (-angle); // negate if in quad III or IV
	else
		return (angle);
}

int Math::rad2quadrant(double angle)
{
	return (int)round(angle / (M_PI / 2.0));
}
