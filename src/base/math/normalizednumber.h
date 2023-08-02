#ifndef MATH_NORMALIZEDNUMBER
#define MATH_NORMALIZEDNUMBER

namespace Math
{

class NormalizedNumber
{
public:
	bool positive;
	double coefficient;
	int exponent;
	double base;
	NormalizedNumber(bool positive,
	    double coefficient,
	    int exponent,
	    double base);
	NormalizedNumber(double value, double base);
	[[nodiscard]] double value() const;
	void setValue(double value);
	[[nodiscard]] double sign() const
	{
		return positive ? 1.0 : -1.0;
	}
	[[nodiscard]] double signedCoef() const
	{
		return positive ? coefficient : -coefficient;
	}
	void setExponent(int exp);

private:
};

class ScientificNumber : public NormalizedNumber
{
public:
	template <typename... T>
	ScientificNumber(T... p) : NormalizedNumber(p..., 10)
	{}
};

class EngineeringNumber : public NormalizedNumber
{
public:
	template <typename... T>
	EngineeringNumber(T... p) : NormalizedNumber(p..., 1000)
	{}
};

}

#endif
