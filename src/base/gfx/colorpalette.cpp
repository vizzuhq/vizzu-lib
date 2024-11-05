#include "colorpalette.h"

#include <cstddef>
#include <initializer_list>
#include <string>

#include "base/text/smartstring.h"

#include "color.h"

namespace Gfx
{

ColorPalette::ColorPalette(std::initializer_list<Color> colors) :
    colors(colors)
{}

ColorPalette::ColorPalette(const std::string &string)
{
	auto colorList =
	    Text::SmartString::split(string, ' ', true, "()");
	colors.reserve(colorList.size());
	for (const auto &color : colorList) {
		colors.emplace_back(Color::fromString(color));
	}
}

ColorPalette::operator std::string() const
{
	return Text::SmartString::join<' '>(colors);
}

Color ColorPalette::operator[](unsigned index) const
{
	return colors.empty() ? Color() : colors[index % colors.size()];
}

Color &ColorPalette::operator[](unsigned index)
{
	if (index >= colors.size()) colors.resize(index + 1);

	return colors[index];
}

ColorPalette::Citerator ColorPalette::begin() const
{
	return colors.begin();
}

ColorPalette::Citerator ColorPalette::end() const
{
	return colors.end();
}

size_t ColorPalette::size() const { return colors.size(); }

}