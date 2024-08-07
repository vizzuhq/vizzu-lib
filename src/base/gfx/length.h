#ifndef GFX_LENGTH
#define GFX_LENGTH

#include <stdexcept>
#include <string>

#include "base/math/floating.h"

namespace Gfx
{

class Length
{
public:
	double absolute{};
	double relative{};
	double emphemeral{};

	static Length Absolute(double value)
	{
		return Length{value, 0, 0};
	}
	static Length Relative(double value)
	{
		return Length{0, value, 0};
	}
	static Length Emphemeral(double value)
	{
		return Length{0, 0, value};
	}

	static Length fromString(const std::string &s);

	constexpr static auto is_zero = Math::Floating::is_zero;

	[[nodiscard]] bool isAbsolute() const
	{
		return is_zero(relative) && is_zero(emphemeral);
	}
	[[nodiscard]] bool isRelative() const
	{
		return is_zero(absolute) && is_zero(emphemeral);
	}
	[[nodiscard]] bool isEmphemeral() const
	{
		return is_zero(absolute) && is_zero(relative);
	}

	[[nodiscard]] double get(double reference, double fontSize) const
	{
		return absolute + relative * reference
		     + emphemeral * fontSize;
	}

	[[nodiscard]] double fromNet(double netReference,
	    double fontSize) const
	{
		if (relative == 1.0)
			throw std::logic_error("padding relative is 1.0");
		return get(netReference, fontSize) / (1 - relative);
	}

	[[nodiscard]] double get() const
	{
		if (isAbsolute()) return absolute;
		throw std::logic_error(
		    "internal error: relative length wo. reference");
	}

	explicit operator std::string() const;

	bool operator==(const Length &other) const
	{
		return absolute == other.absolute
		    && relative == other.relative
		    && emphemeral == other.emphemeral;
	}

	Length operator*(double scale) const
	{
		return Length{absolute * scale,
		    relative * scale,
		    emphemeral * scale};
	}

	Length operator+(const Length &other) const
	{
		return Length{absolute + other.absolute,
		    relative + other.relative,
		    emphemeral + other.emphemeral};
	}
};

}

#endif
