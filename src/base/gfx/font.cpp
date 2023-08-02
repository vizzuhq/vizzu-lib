#include "font.h"

#include <utility>

#include "base/conv/parse.h"
#include "base/conv/tostring.h"

using namespace Gfx;

Font::Weight::Weight(const std::string &str)
{
	if (str == "normal")
		*this = Normal();
	else if (str == "bold")
		*this = Bold();
	else {
		value = Conv::parse<double>(str);
	}
}

Font::Weight::operator std::string() const
{
	if (*this == Normal()) return "normal";
	if (*this == Bold()) return "bold";
	return Conv::toString(value);
}

Font::Weight Font::Weight::operator*(double factor) const
{
	return {static_cast<int>(value * factor)};
};

Font::Weight Font::Weight::operator+(const Font::Weight &other) const
{
	return {value + other.value};
}

bool Font::Weight::operator==(const Font::Weight &other) const
{
	return value == other.value;
}

Font::Font(double size) : style(Gfx::Font::Style::normal)
{
	this->size = size;
}

Font::Font(std::string family,
    Style style,
    Weight weight,
    double size) :
    family(std::move(family)),
    style(style),
    weight(weight),
    size(size)
{}

bool Font::operator==(const Font &other) const
{
	return size == other.size && style == other.style
	    && weight == other.weight && family == other.family;
}

std::string Font::toCSS() const
{
	std::string res;

	if (style == Style::italic)
		res += "italic ";
	else if (style == Style::oblique)
		res += "oblique ";

	if (weight != Weight::Normal()) res += static_cast<std::string>(weight) + " ";

	res += std::to_string(size) + "px ";
	res += family;

	return res;
}
