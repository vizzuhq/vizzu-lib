#ifndef GFX_FONT
#define GFX_FONT

#include <cstdint>
#include <string>

namespace Gfx
{

class Font
{
public:
	class Weight
	{
	public:
		static Weight Normal() { return Weight{400}; }
		static Weight Bold() { return Weight{700}; }
		Weight() : value(Normal().value) {}
		explicit Weight(int value) : value(value) {}
		explicit Weight(const std::string &str);
		explicit operator int() const { return value; };
		explicit operator std::string() const;
		Weight operator*(double factor) const;
		Weight operator+(const Weight &other) const;
		bool operator==(const Weight &other) const;

	private:
		int value;
	};

	enum class Style : std::uint8_t { normal, italic, oblique };

	std::string family;
	Style style;
	Weight weight;
	double size;

	explicit Font(double size = 0);
	Font(std::string family, Style style, Weight weight, double size);
	Font(const Font &) = default;
	Font(Font &&) = default;
	Font &operator=(const Font &) = default;
	Font &operator=(Font &&) = default;
	[[nodiscard]] std::string toCSS() const;

	bool operator==(const Font &other) const;
};

}

#endif
