#ifndef GFX_LENGTH
#define GFX_LENGTH

#include <string>
#include <stdexcept>

#include "base/text/valueunit.h"

namespace Gfx
{

class Length
{
public:
	double absolute;
	double relative;
	double emphemeral;

	Length() : absolute(0), relative(0), emphemeral(0) {}

	static Length Absolute(double value) { return Length(value, 0, 0); }
	static Length Relative(double value) { return Length(0, value, 0); }
	static Length Emphemeral(double value) { return Length(0, 0, value); }

	Length(double abs, double rel = 0.0, double emphemeral = 0.0) : 
		absolute(abs), relative(rel), emphemeral(emphemeral)
	{}

	explicit Length(const std::string &s);

	bool isAbsolute() const { return relative == 0.0 && emphemeral == 0.0; }
	bool isRelative() const { return absolute == 0.0 && emphemeral == 0.0; }
	bool isEmphemeral() const { return absolute == 0.0 && relative == 0.0; }

	double get(double reference, double fontSize) const {
		return absolute + relative * reference + emphemeral * fontSize;
	}

	double get() const {
		if (isAbsolute()) return absolute;
		throw std::logic_error("internal error: relative length wo. reference");
	}

	explicit operator std::string() const;

	bool operator==(const Length &other) const {
		return absolute == other.absolute
			&& relative == other.relative
			&& emphemeral == other.emphemeral;
	}

	Length operator*(double scale) const {
		return Length(absolute * scale, relative * scale, emphemeral * scale);
	}

	Length operator+(const Length &other) const {
		return Length(
			absolute + other.absolute, 
			relative + other.relative,
			emphemeral + other.emphemeral);
	}
};

}

#endif
