#ifndef MATH_NORMALIZEDNUMBER
#define MATH_NORMALIZEDNUMBER

namespace Math
{

class NormalizedNumber
{
public:
	bool positive{};
	double coefficient{};
	int exponent{};
	double base;
	NormalizedNumber(bool positive,
	    double coefficient,
	    int exponent,
	    double base);
	NormalizedNumber(double value, double base);
	[[nodiscard]] double value() const;
	[[nodiscard]] double sign() const
	{
		return positive ? 1.0 : -1.0;
	}
	[[nodiscard]] double signedCoef() const
	{
		return positive ? coefficient : -coefficient;
	}
	void setExponent(int exp);
};

class ScientificNumber : public NormalizedNumber
{
public:
	template <typename... T>
	explicit ScientificNumber(T... p) : NormalizedNumber(p..., 10)
	{}
};

class EngineeringNumber : public NormalizedNumber
{
public:
	template <typename... T>
	explicit EngineeringNumber(T... p) : NormalizedNumber(p..., 1000)
	{}
};

}

#endif
