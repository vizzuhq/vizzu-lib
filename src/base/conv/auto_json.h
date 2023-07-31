#ifndef BASE_AUTO_JSON_H
#define BASE_AUTO_JSON_H

#include <initializer_list>
#include <span>
#include <string>
#include <string_view>

#include "base/refl/auto_struct.h"
#include "base/text/smartstring.h"

#include "tostring.h"

namespace Conv
{
struct Json
{
	void pre(const std::initializer_list<std::string_view> &il) const
	{
		auto &j = json.get();
		const auto *from = std::begin(il);

		if (curr) {
			auto [pre, cur] = std::ranges::mismatch(*curr, il);
			const auto *cend = std::end(*curr);
			if (pre != cend) [[likely]]
				j.append(std::distance(pre, std::prev(cend)), '}');
			j += ',';
			from = cur;
		}

		if (const auto *end = std::end(il); from != end) [[likely]] {
			std::for_each(from,
			    std::prev(end),
			    [&j](std::string_view sv)
			    {
				    j += '\"';
				    j += Text::SmartString::escape(std::string{sv});
				    j += "\":{";
			    });
			j += '\"';
			j += Text::SmartString::escape(
			    std::string{*std::prev(end)});
			j += "\":";
		}
	}

	template <class T> inline void primitive(T &&val)
	{
		auto &j = json.get();
		if constexpr (std::is_arithmetic_v<
		                  std::remove_reference_t<T>>) {
			j += toString(std::forward<T>(val));
		}
		else {
			j += '\"';
			j += Text::SmartString::escape(
			    toString(std::forward<T>(val)));
			j += '\"';
		}
	}

	template <class T>
	inline auto operator()(T &&val,
	    const std::initializer_list<std::string_view> &il)
	    -> std::void_t<decltype(toString(std::forward<T>(val)))>
	{
		pre(il);
		primitive(std::forward<T>(val));
		curr = &il;
	}

	template <class T> inline void array(T &&val)
	{
		auto &j = json.get();
		j += '[';
		bool not_first = false;
		for (const auto &e : val) {
			if (std::exchange(not_first, true)) j += ',';
			if constexpr (requires { toString(e); }) { primitive(e); }
			else if constexpr (std::ranges::range<decltype(e)>) {
				array(e);
			}
			else {
				Refl::visit(Json{json}, e);
			}
		}
		j += ']';
	}

	template <class T>
	inline auto operator()(T &&val,
	    const std::initializer_list<std::string_view> &il)
	    -> std::enable_if_t<std::ranges::range<T>>
	{
		pre(il);
		array(std::forward<T>(val));
		curr = &il;
	}

	Json(std::reference_wrapper<std::string> json) : json(json)
	{
		json.get() += '{';
	}

	~Json()
	{
		auto &j = json.get();
		if (curr)
			j.append(std::distance(std::begin(*curr),
			    std::prev(std::end(*curr))), '}');
		j += '}';
	}
	std::reference_wrapper<std::string> json;
	const std::initializer_list<std::string_view> *curr{};
};

template <class T> std::string toJson(T &&v)
{
	std::string res;
	Refl::visit(Json{std::ref(res)}, v);
	return res;
}
}

#endif
