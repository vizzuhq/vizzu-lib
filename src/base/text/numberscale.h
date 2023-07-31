#ifndef TEXT_NUMBERSCALE
#define TEXT_NUMBERSCALE

#include <string>
#include <vector>

namespace Text
{

enum class PrefixType {
	SISymbol,
	shortScaleSymbolUS,
	shortScaleSymbolUK
};

class NumberScale
{
public:
	static const NumberScale siSymbols;

	explicit NumberScale(PrefixType type);
	explicit NumberScale(std::string s);
	explicit operator std::string() const;
	[[nodiscard]] std::string at(std::size_t index) const;
	[[nodiscard]] std::size_t size() const { return prefixes.size(); }

	bool operator==(const NumberScale &other) const = default;
private:
	std::vector<std::string> prefixes;
};

}

#endif
