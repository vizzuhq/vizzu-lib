#ifndef TEXT_NUMBERSCALE
#define TEXT_NUMBERSCALE

#include <cstdint>
#include <string>
#include <vector>

namespace Text
{

enum class PrefixType : std::uint8_t {
	SISymbol,
	shortScaleSymbolUS,
	shortScaleSymbolUK
};

class NumberScale
{
public:
	explicit NumberScale(PrefixType type = PrefixType::SISymbol);
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
