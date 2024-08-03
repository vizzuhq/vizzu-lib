#ifndef ANIM_EASINGFUNC
#define ANIM_EASINGFUNC

#include <cmath>
#include <numbers>

namespace Anim
{

using EaseFuncBase = double (*)(double);

struct EaseFunc
{
	template <EaseFuncBase FuncBase> static double in(double x)
	{
		return FuncBase(x);
	}

	template <EaseFuncBase FuncBase> static double out(double x)
	{
		return 1 - FuncBase(1 - x);
	}

	template <EaseFuncBase FuncBase> static double inOut(double x)
	{
		return x < 0.5 ? FuncBase(2 * x) / 2
		               : 1 - FuncBase(2 * (1 - x)) / 2;
	}

	template <EaseFuncBase FuncBase> static double middle(double x)
	{
		return x < 0.5 ? (1 - FuncBase(1 - 2 * x)) / 2
		               : (1 + FuncBase(2 * x - 1)) / 2;
	}

	static double sine(double x)
	{
		return 1.0 - cos(x * std::numbers::pi / 2.0);
	}
	static double quad(double x) { return x * x; }
	static double cubic(double x) { return x * x * x; }
	static double quart(double x) { return x * x * x * x; }
	static double quint(double x) { return x * x * x * x * x; }
	static double expo(double x)
	{
		return x == 0 ? 0 : pow(2, 10 * x - 10);
	}
	static double circ(double x) { return 1 - sqrt(1 - x * x); }

	static double back(double x)
	{
		const double c1 = 1.70158;
		const double c3 = c1 + 1;
		return c3 * x * x * x - c1 * x * x;
	}

	static double elastic(double x)
	{
		const double c4 = 2.0 * std::numbers::pi / 3.0;
		return 0 < x && x < 1
		         ? -pow(2, 10 * x - 10) * sin((x * 10 - 10.75) * c4)
		         : x;
	}
};
}

#endif
