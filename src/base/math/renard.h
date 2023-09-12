#ifndef MATH_RENARD
#define MATH_RENARD

#include <vector>

namespace Math
{

class Renard
{
public:
	static Renard R3();
	static Renard R5();
	explicit Renard(const std::vector<double> &numbers) :
	    numbers(numbers)
	{}
	double ceil(double value);
	double floor(double value);

private:
	const std::vector<double> &numbers;
};

}

#endif
