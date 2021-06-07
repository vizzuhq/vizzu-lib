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
	Renard(const std::vector<double> &numbers) : numbers(numbers) {}
	double ceil(double value);
	double floor(double value);

private:
	static const std::vector<double> R3Numbers;
	static const std::vector<double> R5Numbers;

	const std::vector<double> &numbers;
};

}

#endif

