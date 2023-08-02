#ifndef GFX_FONT
#define GFX_FONT

#include <string>

namespace Gfx
{

class Font
{
public:
	class Weight
	{
	public:
		static Weight Normal() { return {400}; }
		static Weight Bold() { return {700}; }
		Weight() { *this = Normal(); }
		Weight(int value) : value(value) {}
		explicit Weight(const std::string &str);
		operator int() const { return value; };
		operator std::string() const;
		Weight operator*(double factor) const;
		Weight operator+(const Weight &other) const;
		bool operator==(const Weight &other) const;

	private:
		int value;
	};

	enum class Style { normal, italic, oblique };

	std::string family;
	Style style;
	Weight weight;
	double size;

	Font(double size = 0);
	Font(std::string family,
	    Style style,
	    Weight weight,
	    double size);
	[[nodiscard]] std::string toCSS() const;

	bool operator==(const Font &other) const;
};

}

#endif
