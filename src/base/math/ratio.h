#ifndef MATH_RATIO
#define MATH_RATIO

#include <cmath>

namespace Math
{

template <int denom> class Ratio
{
public:
	explicit Ratio(int i) : count(i) {}
	explicit Ratio(double f) : count(static_cast<int>(round(f * denom))) {}

	operator double() const { return static_cast<double>(count) / denom; }

	bool operator==(const Ratio<denom> &other)
	{
		return count == other.count;
	}

	Ratio<denom> &operator+=(const Ratio<denom> &other)
	{
		count += other.count;
		return *this;
	}

	Ratio<denom> &operator*=(int multiplier)
	{
		count *= multiplier;
		return *this;
	}

	Ratio<denom> operator+(const Ratio<denom> &other)
	{
		return Ratio(count + other.count);
	}

	Ratio<denom> operator*(int multiplier) const
	{
		return Ratio(count * multiplier);
	}

	bool operator<(const Ratio<denom> &other)
	{
		return count < other.count;
	}

private:
	int count;
};

}

#endif
