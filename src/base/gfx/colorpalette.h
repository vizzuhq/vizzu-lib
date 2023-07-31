#ifndef GFX_COLORPALETTE
#define GFX_COLORPALETTE

#include <initializer_list>
#include <vector>

#include "color.h"

namespace Gfx
{

class ColorPalette
{
public:
	using Colors = std::vector<Gfx::Color>;
	using Citerator = Colors::const_iterator;

	ColorPalette() = default;
	ColorPalette(const ColorPalette &) = default;
	ColorPalette(ColorPalette &&) = default;
	ColorPalette &operator=(const ColorPalette &) = default;
	ColorPalette &operator=(ColorPalette &&) = default;

	ColorPalette(std::initializer_list<Gfx::Color> colors);
	explicit ColorPalette(const std::string &string);
	explicit operator std::string() const;

	Gfx::Color operator[](unsigned index) const;
	Gfx::Color &operator[](unsigned index);
	[[nodiscard]] Citerator begin() const;
	[[nodiscard]] Citerator end() const;

	[[nodiscard]] size_t size() const;

	bool operator==(const ColorPalette&) const = default;
private:
	Colors colors;
};

}

#endif
