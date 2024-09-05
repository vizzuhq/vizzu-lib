#ifndef GFX_COLORTRANSFORM
#define GFX_COLORTRANSFORM

#include <functional>

#include "base/gfx/color.h"

namespace Gfx
{

class ColorTransform
{
public:
	ColorTransform() = default;
	static ColorTransform fromString(const std::string &);
	explicit operator std::string() const;

	static ColorTransform OverrideColor(Color overrideColor);
	static ColorTransform Grayscale(double factor);
	static ColorTransform Lightness(double factor);
	static ColorTransform Opacity(double factor);
	static ColorTransform None();

	Color operator()(const Color &color) const;

	ColorTransform operator*(double value) const;
	ColorTransform operator+(const ColorTransform &other) const;

	bool operator==(const ColorTransform &other) const;

private:
	using Convert = std::function<Color(const Color &)>;
	std::string code;
	Convert convert;

	ColorTransform(Convert convert, std::string code);
};

}

#endif
