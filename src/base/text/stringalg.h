#ifndef TEXT_STRINGALG
#define TEXT_STRINGALG

#include "character.h"

namespace Text
{

struct String
{
	static constexpr std::size_t find(std::string_view view, char c)
	{
		for (auto i = 0u; i < view.size(); i++)
			if (view[i] == c) return i;
		return std::string_view::npos;
	}

	static constexpr std::size_t rfind(std::string_view view, char c)
	{
		for (auto i = 0u; i < view.size(); i++)
			if (view[view.size() - 1 - i] == c) return i;
		return std::string_view::npos;
	}

	static constexpr auto trim(std::string_view view)
	{
		while (!view.empty() && Character::isWhiteSpace(view.front()))
			view.remove_prefix(1);

		while (!view.empty() && Character::isWhiteSpace(view.back()))
			view.remove_suffix(1);

		return view;
	}

	static constexpr auto popTill(std::string_view view, char c)
	{
		auto pos = Text::String::find(view, c);
		if (pos == std::string_view::npos)
			return view;
		else
			return view.substr(0, pos + 1);
	}

	static constexpr auto chop(std::string_view view, char c)
	{
		if (!view.empty() && view.back() == c)
			view.remove_suffix(1);
		return view;
	}
};

}

#endif
