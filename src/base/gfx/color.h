#ifndef GFX_COLOR
#define GFX_COLOR

#include <base/math/floating.h>
#include <cmath>
#include <cstdint>
#include <ctgmath>
#include <string>

#include "base/math/interpolation.h"

namespace Gfx
{

struct Color
{
	double red;
	double green;
	double blue;
	double alpha;

	Color() { red = green = blue = alpha = 0.0; }

	Color(double red,
	    double green,
	    double blue,
	    double alpha = 1.0) noexcept :
	    red(red),
	    green(green),
	    blue(blue),
	    alpha(alpha)
	{}

	Color(const Color &) = default;
	Color &operator=(const Color &) = default;

	explicit Color(const std::string &string);

	static Color RGB(uint32_t rgb);
	static Color RGBA(uint32_t rgba);
	static Color
	RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

	static Color Gray(double intensity, double alpha = 1.0)
	{
		return {intensity, intensity, intensity, alpha};
	}

	static Color Transparent() { return {0.0, 0.0, 0.0, 0.0}; }
	static Color White() { return Color::Gray(1.0); }
	static Color Black() { return Color::Gray(0.0); }

	Color operator*(double alpha) const
	{
		Color color = *this;
		color.alpha *= alpha;
		return color;
	}

	Color operator+(const Color &other) const
	{
		Color color;
		color.red = blendChannel(red, alpha, other.red, other.alpha);
		color.green =
		    blendChannel(green, alpha, other.green, other.alpha);
		color.blue =
		    blendChannel(blue, alpha, other.blue, other.alpha);
		color.alpha = alpha + other.alpha;
		return color;
	}

	bool operator==(const Color &other) const = default;

	[[nodiscard]] double intensity() const
	{
		return (red + green + blue) / 3.0;
	}

	[[nodiscard]] Color desaturate(double factor = 1) const
	{
		if (factor == 1) return Gray(intensity(), alpha);

		return Math::interpolate(*this,
		    Gray(intensity(), alpha),
		    factor);
	}

	[[nodiscard]] Color invert(double factor = 1) const
	{
		auto inverted = Color(1 - red, 1 - green, 1 - blue, alpha);
		if (factor == 1) return inverted;
		return Math::interpolate(*this, inverted, factor);
	}

	[[nodiscard]] Color transparent(double alpha = 0.0) const
	{
		auto res = *this;
		res.alpha = alpha;
		return res;
	}

	[[nodiscard]] Color darkened(double factor = 0.0) const
	{
		if (factor < 0.0) factor = 0.0;
		if (factor > 1.0) factor = 1.0;
		return Math::interpolate(*this,
		    Black().transparent(alpha),
		    factor);
	}

	[[nodiscard]] Color lightened(double factor = 0.0) const
	{
		if (factor < 0.0) factor = 0.0;
		if (factor > 1.0) factor = 1.0;
		return Math::interpolate(*this,
		    White().transparent(alpha),
		    factor);
	}

	[[nodiscard]] Color lightnessScaled(double factor = 0.0) const
	{
		if (factor == 0.0) return *this;
		if (factor > 0) return lightened(factor);
		return darkened(-factor);
	}

	[[nodiscard]] uint8_t getRedByte() const
	{
		return static_cast<int>(fixed(red) * 255);
	}
	[[nodiscard]] uint8_t getGreenByte() const
	{
		return static_cast<int>(fixed(green) * 255);
	}
	[[nodiscard]] uint8_t getBlueByte() const
	{
		return static_cast<int>(fixed(blue) * 255);
	}
	[[nodiscard]] uint8_t getAlphaByte() const
	{
		return static_cast<int>(fixed(alpha) * 255);
	}

	[[nodiscard]] uint32_t getRGBA32() const
	{
		return static_cast<uint32_t>(getRedByte() << 24U)
		     | static_cast<uint32_t>(getGreenByte() << 16U)
		     | static_cast<uint32_t>(getBlueByte() << 8U)
		     | getAlphaByte();
	}

	[[nodiscard]] bool isOpaque() const { return alpha == 1.0; }
	[[nodiscard]] bool isTransparent() const { return alpha == 0.0; }

	explicit operator std::string() const;

private:
	static double fixed(double x)
	{
		return std::clamp(x, 0.0, 1.0, Math::Floating::less);
	}

	static double
	blendChannel(double v0, double a0, double v1, double a1)
	{
		if (a0 + a1 == 0) return (v0 + v1) / 2.0;
		return (v0 * a0 + v1 * a1) / (a0 + a1);
	}
};

}

#endif
