#ifndef TEXT_CHARACTER
#define TEXT_CHARACTER

#include <stdexcept>

namespace Text
{

struct Character
{
	static uint8_t hex(const char *hex)
	{
		return (fromHex(*hex) << 4) + fromHex(*(hex + 1));
	}

	static constexpr bool isDigit(char ch)
	{
		return ch >= '0' && ch <= '9';
	}

	static constexpr bool isSpace(char ch) { return ch == ' '; }

	static constexpr char toUpper(char ch)
	{
		return ch >= 'a' && ch <= 'z' ? ch - 'a' + 'Z' : ch;
	}

	static constexpr char toNumber(char ch)
	{
		if (ch >= '0' && ch <= '9')
			return ch - '0';
		throw std::logic_error("invalid decimal digit");
	}

	static constexpr char toHex(char ch)
	{
		if (ch >= 0 && ch < 16)
			return "0123456789ABCDEF"[static_cast<unsigned char>(ch)];

		throw std::out_of_range(
		    "invalid 1 digit hexadecimal number");
	}

	static void toHex(uint8_t ch, char *res)
	{
		*res = toHex((ch >> 4) & 0x0F);
		*(res + 1) = toHex(ch & 0x0F);
	}

	static constexpr uint8_t fromHex(char ch)
	{
		if (ch >= '0' && ch <= '9') return ch - '0';
		if (ch >= 'a' && ch <= 'f') return 10 + ch - 'a';
		if (ch >= 'A' && ch <= 'F') return 10 + ch - 'A';
		throw std::invalid_argument("invalid hexadecimal digit");
	}
};

}

#endif
