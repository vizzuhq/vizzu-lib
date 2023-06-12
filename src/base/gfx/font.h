#ifndef GFX_FONT
#define GFX_FONT

#include <string>

#include "base/refl/enum.h"

namespace Gfx
{

class Font
{
public:
	class Weight
	{
	public:
		static Weight Normal() { return Weight(400); }
		static Weight Bold() { return Weight(700); }
		Weight() { *this = Normal(); }
		Weight(int value) : value(value) {}
		explicit Weight(const std::string &str);
		operator int() const { return value; };
		operator std::string() const;
		Weight operator*(double factor) const;
		Weight operator+(const Weight &other) const;
		bool operator==(const Weight &other) const;

	private:
		int value;
	};

	class Enum(Style)(normal,italic,oblique);

	std::string family;
	Style style;
	Weight weight;
	double size;

	Font(double size = 0);
	Font(const std::string &family,
	    Style style,
	    Weight weight,
	    double size);
	std::string toCSS() const;

	bool operator==(const Font &other) const;
};

}

#endif
