
#include "colorpalette.h"

#include "base/text/smartstring.h"

Gfx::ColorPalette::ColorPalette(
    std::initializer_list<Gfx::Color> colors) :
    colors(colors)
{}

Gfx::ColorPalette::ColorPalette(const std::string &string)
{
	auto colorList =
	    Text::SmartString::split(string, ' ', true, "()");
	colors.reserve(colorList.size());
	for (const auto &color : colorList) {
		colors.emplace_back(color);
	}
}

Gfx::ColorPalette::operator std::string() const
{
	std::string res;
	for (const auto &color : colors) {
		res += (!res.empty() ? " " : "");
		res += static_cast<std::string>(color);
	}
	return res;
}

Gfx::Color Gfx::ColorPalette::operator[](unsigned index) const
{
	return colors.empty() ? Gfx::Color()
	                      : colors[index % colors.size()];
}

Gfx::Color &Gfx::ColorPalette::operator[](unsigned index)
{
	if (index >= colors.size()) colors.resize(index + 1);

	return colors[index];
}

Gfx::ColorPalette::Citerator Gfx::ColorPalette::begin() const
{
	return colors.begin();
}

Gfx::ColorPalette::Citerator Gfx::ColorPalette::end() const
{
	return colors.end();
}

size_t Gfx::ColorPalette::size() const { return colors.size(); }
