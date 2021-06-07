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

	static ColorTransform OverrideColor(const Gfx::Color &overrideColor);
	static ColorTransform Grayscale(double factor);
	static ColorTransform Lightness(double factor);

	Gfx::Color operator()(const Gfx::Color &color) const;

	ColorTransform operator*(double value) const;
	ColorTransform operator+(const ColorTransform &other) const;

	bool operator==(const ColorTransform &other) const;

private:
	typedef std::function<Gfx::Color(const Gfx::Color &)> Convert;
	std::string code;
	Convert convert;

	ColorTransform(const Convert &convert, const std::string &code);
};

}

#endif
