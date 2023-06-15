#ifndef TEXT_NUMBERSCALE
#define TEXT_NUMBERSCALE

#include <string>
#include <vector>

#include "base/refl/enum.h"

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
	std::string at(std::size_t index) const;
	std::size_t size() const { return prefixes.size(); }

private:
	std::vector<std::string> prefixes;
};

}

#endif
