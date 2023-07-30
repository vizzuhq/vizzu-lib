#ifndef ANIM_EASING
#define ANIM_EASING

#include <functional>
#include <string>

namespace Anim
{

class Easing
{
public:
	using Function = std::function<double (double)>;

	static double none(double) { return 0; }
	static double start(double) { return 1; }
	static double end(double t) { return t == 1 ? 1 : 0; }
	static double linear(double x) { return x; }

	Easing() = default;
	Easing(Function func) : func(std::move(func)) {}
	explicit Easing(const std::string &name);

	double operator()(double x) const
	{
		if (x < 0) return 0;
		if (x > 1) return 1;
		if (func) return func(x);
		return x;
	}

private:
	std::function<double(double)> func;
};

}

#endif
