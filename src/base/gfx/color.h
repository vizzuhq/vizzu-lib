#ifndef GFX_COLOR
#define GFX_COLOR

#include <cstdint>
#include <cmath>
#include <ctgmath>
#include <string>

#include "base/math/interpolation.h"

// qt opengl headers define these unfortunatelly
#undef RGB
#undef RGBA

namespace Gfx
{

struct Color
{
	double red;
	double green;
	double blue;
	double alpha;

	Color() {
		red = green = blue = alpha = 0.0;
	}

	Color(double red, double green, double blue, double alpha = 1.0)
		: red(red), green(green), blue(blue), alpha(alpha)
	{}

	Color(const std::string &string);

	static Color RGB(uint32_t rgb);
	static Color RGBA(uint32_t rgba);
	static Color RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

	static Color Gray(double intensity, double alpha = 1.0) {
		return Color(intensity, intensity, intensity, alpha);
	}

	static Color Transparent() { return Color(0.0, 0.0, 0.0, 0.0); }
	static Color White() { return Color::Gray(1.0); }
	static Color Black() { return Color::Gray(0.0); }

	Color operator*(double alpha) const {
		Color color = *this;
		color.alpha *= alpha;
		return color;
	}

	Color operator+(const Color &other) const
	{
		Color color;
		color.red = blendChannel(red, alpha, other.red, other.alpha);
		color.green = blendChannel(green, alpha, other.green, other.alpha);
		color.blue = blendChannel(blue, alpha, other.blue, other.alpha);
		color.alpha = alpha + other.alpha;
		return color;
	}

	bool operator==(const Color &other) const {
		return red == other.red
				&& green == other.green
				&& blue == other.blue
				&& alpha == other.alpha;
	}

	double intensity() const
	{
		return (red + green + blue) / 3.0;
	}

	Color desaturate(double factor = 1) const
	{
		if (factor == 1)
			return Gray(intensity(), alpha);
		else
			return Math::interpolate(*this, Gray(intensity(), alpha), factor);
	}

	Color invert(double factor = 1) const
	{
		auto inverted = Color(1-red, 1-green, 1-blue, alpha);
		if (factor == 1)
			return inverted;
		else
			return Math::interpolate(*this, inverted, factor);
	}

	Color transparent(double alpha = 0.0) const
	{
		auto res = *this;
		res.alpha = alpha;
		return res;
	}

	Color darkened(double factor = 0.0) const
	{
		if (factor < 0.0) factor = 0.0;
		if (factor > 1.0) factor = 1.0;
		return Math::interpolate(*this, Black().transparent(alpha), factor);
	}

	Color lightened(double factor = 0.0) const
	{
		if (factor < 0.0) factor = 0.0;
		if (factor > 1.0) factor = 1.0;
		return Math::interpolate(*this, White().transparent(alpha), factor);
	}

	Color lightnessScaled(double factor = 0.0) const
	{
		if (factor == 0.0) return *this;
		if (factor > 0) return lightened(factor);
		else return darkened(-factor);
	}

	uint8_t getRedByte() const { return (int)(fixed(red) * 255); }
	uint8_t getGreenByte() const { return (int)(fixed(green) * 255); }
	uint8_t getBlueByte() const { return (int)(fixed(blue) * 255); }
	uint8_t getAlphaByte() const { return (int)(fixed(alpha) * 255); }

	uint32_t getRGBA32() const {
		return	(((uint32_t)getRedByte()) << 24)
				| (getGreenByte() << 16)
				| (getBlueByte() << 8)
				| getAlphaByte();
	}

	bool isOpaque() const { return alpha == 1.0; }
	bool isTransparent() const { return alpha == 0.0; }

	explicit operator std::string() const;

private:

	static double fixed(double x)
	{
		return std::isnan(x) ? 0 :
			   x > 1    ? 1 :
			   x < 0    ? 0 :
						  x ;
	}

	static double blendChannel(double v0, double a0, double v1, double a1)
	{
		if (a0 + a1 == 0) return (v0 + v1) / 2.0;
		return (v0 * a0 + v1 * a1) / (a0 + a1);
	}
};

}

#endif
