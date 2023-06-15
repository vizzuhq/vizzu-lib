#ifndef TEXT_STRINGALG
#define TEXT_STRINGALG

#include "character.h"

namespace Text
{

struct String
{

	static constexpr auto trim(std::string_view view)
	{
		while (!view.empty() && Character::isWhiteSpace(view.front()))
			view.remove_prefix(1);

		while (!view.empty() && Character::isWhiteSpace(view.back()))
			view.remove_suffix(1);

		return view;
	}

	static constexpr auto chop(std::string_view view, char c)
	{
		if (!view.empty() && view.back() == c) view.remove_suffix(1);
		return view;
	}
};

}

#endif
