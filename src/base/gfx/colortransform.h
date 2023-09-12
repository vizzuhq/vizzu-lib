#ifndef GFX_COLORTRANSFORM
#define GFX_COLORTRANSFORM

#include <functional>

#include "base/gfx/color.h"

namespace Gfx
{

class ColorTransform
{
public:
	explicit ColorTransform(const std::string &code = std::string());
	explicit operator std::string() const;

	static ColorTransform OverrideColor(Gfx::Color overrideColor);
	static ColorTransform Grayscale(double factor);
	static ColorTransform Lightness(double factor);
	static ColorTransform Opacity(double factor);
	static ColorTransform None();

	Gfx::Color operator()(const Gfx::Color &color) const;

	ColorTransform operator*(double value) const;
	ColorTransform operator+(const ColorTransform &other) const;

	bool operator==(const ColorTransform &other) const;

private:
	using Convert = std::function<Gfx::Color(const Gfx::Color &)>;
	std::string code;
	Convert convert;

	ColorTransform(Convert convert, std::string code);
};

}

#endif
