#ifndef GFX_COLORPALETTE
#define GFX_COLORPALETTE

#include <vector>
#include <initializer_list>

#include "color.h"

namespace Gfx
{

class ColorPalette
{
public:
	typedef std::vector<Gfx::Color> Colors;
	typedef Colors::const_iterator Citerator;

	ColorPalette() = default;
	ColorPalette(const ColorPalette&) = default;
	ColorPalette(std::initializer_list<Gfx::Color> colors);
	explicit ColorPalette(const std::string &string);
	explicit operator std::string() const;

	Gfx::Color operator[](unsigned index) const;
	Gfx::Color &operator[](unsigned index);
	Citerator begin() const;
	Citerator end() const;

	size_t size() const;

private:
	Colors colors;
};

}

#endif
