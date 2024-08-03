#include "color.h"

#include <cstdint>
#include <stdexcept>
#include <string>

#include "base/conv/parse.h"
#include "base/text/character.h"
#include "base/text/funcstring.h"

namespace Gfx
{

Color::operator std::string() const
{
	std::string res(9, '\0');
	res[0] = '#';
	Text::Character::toHex(getRedByte(), res.data() + 1);
	Text::Character::toHex(getGreenByte(), res.data() + 3);
	Text::Character::toHex(getBlueByte(), res.data() + 5);
	Text::Character::toHex(getAlphaByte(), res.data() + 7);
	return res;
}

Color::Color(const std::string &string)
{
	if (string.empty()) { *this = Transparent(); }
	else if (string[0] == '#' && string.size() == 9) {
		red = Text::Character::hex(&string[1]) / 255.0;
		green = Text::Character::hex(&string[3]) / 255.0;
		blue = Text::Character::hex(&string[5]) / 255.0;
		alpha = Text::Character::hex(&string[7]) / 255.0;
	}
	else if (string[0] == '#' && string.size() == 7) {
		red = Text::Character::hex(&string[1]) / 255.0;
		green = Text::Character::hex(&string[3]) / 255.0;
		blue = Text::Character::hex(&string[5]) / 255.0;
		alpha = 1.0;
	}
	else if (string[0] == '#' && string.size() == 4) {
		auto r = Text::Character::fromHex(string[1]);
		auto g = Text::Character::fromHex(string[2]);
		auto b = Text::Character::fromHex(string[3]);
		red = ((r << 4U) + r) / 255.0;
		green = ((g << 4U) + g) / 255.0;
		blue = ((b << 4U) + b) / 255.0;
		alpha = 1.0;
	}
	else if (const Text::FuncString f(string, false); !f.isEmpty()) {
		using Conv::parse;

		if (f.getName() == "rgb") {
			auto ps = f.getParams();
			if (ps.size() != 3)
				throw std::logic_error("invalid color string");
			*this = RGBA(parse<uint32_t>(ps.at(0)),
			    parse<uint32_t>(ps.at(1)),
			    parse<uint32_t>(ps.at(2)));
		}
		else if (f.getName() == "rgba") {
			auto ps = f.getParams();
			if (ps.size() != 4)
				throw std::logic_error("invalid color string");
			*this = RGBA(parse<uint32_t>(ps.at(0)),
			    parse<uint32_t>(ps.at(1)),
			    parse<uint32_t>(ps.at(2)),
			    static_cast<uint8_t>(parse<double>(ps.at(3)) * 255));
		}
		else
			throw std::logic_error("invalid color string");
	}
	else
		throw std::logic_error("invalid color string");
}

Color Color::RGB(uint32_t rgb)
{
	Color c;

	c.blue = (rgb & 0x000000FFU) / 255.0;
	rgb >>= 8U;
	c.green = (rgb & 0x000000FFU) / 255.0;
	rgb >>= 8U;
	c.red = (rgb & 0x000000FFU) / 255.0;

	c.alpha = 1.0;

	return c;
}

Color Color::RGBA(uint32_t rgba)
{
	auto alpha = (rgba & 0x000000FFU) / 255.0;
	auto rgb = rgba >> 8U;
	auto c = RGB(rgb);
	c.alpha = alpha;

	return c;
}

Color Color::RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	return {r / 255.0, g / 255.0, b / 255.0, a / 255.0};
}

}