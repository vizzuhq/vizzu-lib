#ifndef MATH_SIGN
#define MATH_SIGN

namespace Math
{

static inline double sign(double x) { return x > 0 ? 1 : x < 0 ? -1 : 0; }

}

#endif
