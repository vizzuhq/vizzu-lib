#ifndef TEXT_CHARACTER
#define TEXT_CHARACTER

#include <stdexcept>

namespace Text::Character
{
[[nodiscard]] static constexpr bool isDigit(char ch)
{
	return ch >= '0' && ch <= '9';
}

[[nodiscard]] static constexpr bool isSpace(char ch)
{
	return ch == ' ';
}

[[nodiscard]] static constexpr char toUpper(char ch)
{
	return ch >= 'a' && ch <= 'z' ? static_cast<char>(ch - 'a' + 'Z')
	                              : ch;
}

[[nodiscard]] static constexpr char toNumber(char ch)
{
	if (ch >= '0' && ch <= '9') return static_cast<char>(ch - '0');
	throw std::logic_error("invalid decimal digit");
}

[[nodiscard]] static constexpr char toHex(char ch)
{
	if (ch >= 0 && ch < 16)
		return "0123456789ABCDEF"[static_cast<unsigned char>(ch)];

	throw std::out_of_range("invalid 1 digit hexadecimal number");
}

static constexpr void toHex(uint8_t ch, char *res)
{
	*res = toHex(
	    static_cast<char>(static_cast<uint32_t>(ch >> 4U) & 0x0FU));
	*(res + 1) = toHex(static_cast<char>(ch & 0x0FU));
}

[[nodiscard]] static constexpr uint8_t fromHex(char ch)
{
	if (ch >= '0' && ch <= '9') return ch - '0';
	if (ch >= 'a' && ch <= 'f') return 10 + ch - 'a';
	if (ch >= 'A' && ch <= 'F') return 10 + ch - 'A';
	throw std::invalid_argument("invalid hexadecimal digit");
}

[[nodiscard]] static constexpr uint8_t hex(const char *hex)
{
	return (fromHex(*hex) << 4U) + fromHex(*(hex + 1));
}
}

#endif
