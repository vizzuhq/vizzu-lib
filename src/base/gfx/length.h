#ifndef GFX_LENGTH
#define GFX_LENGTH

#include <string>

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

	Length(double abs, double rel) : 
		absolute(abs), relative(rel)
	{}

	explicit Length(const std::string &s);

	double get(double reference) const {
		return absolute + relative * reference;
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
