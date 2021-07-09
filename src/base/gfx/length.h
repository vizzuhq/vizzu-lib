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

	Length() : absolute(0), relative(0) {}

	static Length Absolute(double value) { return Length(value, 0); }
	static Length Relative(double value) { return Length(0, value); }

	Length(double abs, double rel = 0.0) : 
		absolute(abs), relative(rel)
	{}

	explicit Length(const std::string &s);

	bool isAbsolute() const { return relative == 0.0; }
	bool isRelative() const { return absolute == 0.0; }

	double get(double reference) const {
		return absolute + relative * reference;
	}

	double get() const {
		if (isAbsolute()) return absolute;
		throw std::logic_error("internal error: relative length wo. reference");
	}

	explicit operator std::string() const;

	bool operator==(const Length &other) const {
		return absolute == other.absolute
			&& relative == other.relative;
	}

	Length operator*(double scale) const {
		return Length(absolute * scale, relative * scale);
	}

	Length operator+(const Length &other) const {
		return Length(absolute + other.absolute, relative + other.relative);
	}
};

}

#endif
