
#include "colortransform.h"

#include <stdexcept>
#include <string>
#include <utility>

#include "base/text/funcstring.h"
#include "base/text/smartstring.h"

#include "color.h"

namespace Gfx
{

ColorTransform ColorTransform::fromString(const std::string &code)
{
	if (Text::SmartString::trim_view(code) == "none") return None();

	const Text::FuncString func(code);

	if (func.isEmpty()) return {};

	if (func.getParams().size() != 1)
		throw std::logic_error(
		    "invalid color transform parameter count");

	if (func.getName() == "color")
		return OverrideColor(
		    Color::fromString(func.getParams().at(0)));
	if (func.getName() == "lightness")
		return Lightness(std::stod(func.getParams().at(0)));
	if (func.getName() == "grayscale")
		return Grayscale(std::stod(func.getParams().at(0)));
	if (func.getName() == "opacity")
		return Opacity(std::stod(func.getParams().at(0)));
	throw std::logic_error("invalid color transform string");
}

ColorTransform ColorTransform::OverrideColor(Color overrideColor)
{
	return {[=](const Color &)
	    {
		    return overrideColor;
	    },
	    "color(" + std::string{overrideColor} + ")"};
}

ColorTransform ColorTransform::Grayscale(double factor)
{
	return {[=](const Color &color)
	    {
		    return color.desaturate(factor);
	    },
	    "grayscale(" + std::to_string(factor) + ")"};
}

ColorTransform ColorTransform::Lightness(double factor)
{
	return {[=](const Color &color)
	    {
		    return color.lightnessScaled(factor);
	    },
	    "lightness(" + std::to_string(factor) + ")"};
}

ColorTransform ColorTransform::Opacity(double factor)
{
	return {[=](const Color &color)
	    {
		    return color.transparent(factor);
	    },
	    "opacity(" + std::to_string(factor) + ")"};
}

ColorTransform ColorTransform::Fade(double factor)
{
	return {[=](const Color &color)
	    {
		    return color * factor;
	    },
	    "fade(" + std::to_string(factor) + ")"};
}

ColorTransform ColorTransform::None()
{
	return {[=](const Color &color)
	    {
		    return color;
	    },
	    "none"};
}

ColorTransform::ColorTransform(Convert convert, std::string code) :
    code(std::move(code)),
    convert(std::move(convert))
{}

bool ColorTransform::operator==(const ColorTransform &other) const
{
	return code == other.code;
}

ColorTransform ColorTransform::operator*(double value) const
{
	return {[*this, value](const Color &color)
	    {
		    return (*this)(color)*value;
	    },
	    ""};
}

ColorTransform ColorTransform::operator+(
    const ColorTransform &other) const
{
	return {[*this, other](const Color &color)
	    {
		    return (*this)(color) + other(color);
	    },
	    ""};
}

Color ColorTransform::operator()(const Color &color) const
{
	return convert ? convert(color) : color;
}

ColorTransform::operator std::string() const { return code; }

}