#ifndef TEXT_CHARACTER
#define TEXT_CHARACTER

#include <stdexcept>

namespace Text
{

struct Character
{
	static char hex(const char *hex)
	{
		return (fromHex(*hex) << 4) + fromHex(*(hex + 1));
	}

	static constexpr bool isLCLetter(char ch)
	{
		return (ch >= 'a' && ch <= 'z');
	}

	static constexpr bool isUCLetter(char ch)
	{
		return (ch >= 'A' && ch <= 'Z');
	}

	static constexpr bool isLetter(char ch)
	{
		return isLCLetter(ch) || isUCLetter(ch);
	}

	static constexpr bool isAlpha(char ch)
	{
		return isLetter(ch) || (ch == '_');
	}

	static constexpr bool isDigit(char ch)
	{
		return ch >= '0' && ch <= '9';
	}

	static constexpr bool isAlnum(char ch)
	{
		return isAlpha(ch) || isDigit(ch);
	}

	static constexpr bool isSpace(char ch) { return ch == ' '; }

	static constexpr bool isWhiteSpace(char ch) { 
		return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n'; 
	}

	static constexpr bool isHexnum(char ch)
	{
		return isDigit(ch) || (ch >= 'A' && ch <= 'F')
		    || (ch >= 'a' && ch <= 'f');
	}

	static constexpr char toLower(char ch)
	{
		if (ch >= 'A' && ch <= 'Z') return ch - 'A' + 'a';
		else return ch;
	}

	static constexpr char toUpper(char ch)
	{
		if (ch >= 'a' && ch <= 'z') return ch - 'a' + 'Z';
		else return ch;
	}

	static constexpr char toNumber(char ch)
	{
		if (ch >= '0' && ch <= '9') return ch - '0';
		else throw std::logic_error("invalid decimal digit");
	}

	static constexpr char toHex(char ch)
	{
		if (ch >= 0 && ch < 16)
			return "0123456789ABCDEF"[((unsigned)ch)];
		else
			throw std::out_of_range(
			    "invalid 1 digit hexadecimal number");
	}

	static void toHex(char ch, char *res)
	{
		*res = toHex( (ch >> 4) & 0x0F );
		*(res + 1) = toHex(ch & 0x0F);
	}

	static constexpr char fromHex(char ch)
	{
		if (ch >= '0' && ch <= '9') return ch - '0';
		if (ch >= 'a' && ch <= 'f') return 10 + ch - 'a';
		if (ch >= 'A' && ch <= 'F') return 10 + ch - 'A';
		throw std::invalid_argument("invalid hexadecimal digit");
	}
};

}

#endif
