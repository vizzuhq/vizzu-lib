
#include "colorpalette.h"

#include "base/text/smartstring.h"

using namespace Gfx;

ColorPalette::ColorPalette(std::initializer_list<Gfx::Color> colors) :
    colors(colors)
{}

ColorPalette::ColorPalette(const std::string &string)
{
	auto colorList =
	    Text::SmartString::split(string, ' ', true, "()");
	colors.reserve(colorList.size());
	for (const auto &color : colorList) {
		colors.emplace_back(color);
	}
}

ColorPalette::operator std::string() const
{
	std::string res;
	for (const auto &color : colors) {
		res += (!res.empty() ? " " : "");
		res += (std::string)color;
	}
	return res;
}

Gfx::Color ColorPalette::operator[](unsigned index) const
{
	return colors.empty() ? Gfx::Color()
	                      : colors[index % colors.size()];
}

Gfx::Color &ColorPalette::operator[](unsigned index)
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
