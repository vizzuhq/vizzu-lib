#ifndef BASE_AUTO_JSON_H
#define BASE_AUTO_JSON_H

#include <initializer_list>
#include <string>
#include <string_view>

#include "base/refl/auto_struct.h"
#include "base/text/smartstring.h"

#include "tostring.h"

namespace Conv
{
struct Json
{
	void pre(const std::initializer_list<std::string_view> &il)
	{
		const auto *from = std::begin(il);
		const auto *end = std::end(il);

		if (curr) {
			auto [pre, cur] = std::ranges::mismatch(*curr, il);
			if (const auto *cend = std::end(*curr); pre != cend)
			    [[likely]]
				json.append(cend - pre - 1, '}');
			else {
				if (cur == end) {
					throw std::logic_error(
					    "Same object but multiple base classes are "
					    "pure serializable.");
				}
				throw std::logic_error(
					"An already serialized object member is not "
					"serializable.");
			}
			json += ',';
			from = cur;
		}

		if (from != end) [[likely]] {
			for (--end; from != end; ++from) {
				json += '\"';
				json += Text::SmartString::escape(std::string{*from});
				json += "\":{";
			}
			json += '\"';
			json += Text::SmartString::escape(std::string{*end});
			json += "\":";
		}
		else {
			throw std::logic_error("Member of a serializable object "
			                       "are already serialized.");
		}
		curr = &il;
	}

	template <class T> inline void primitive(T &&val)
	{
		if constexpr (std::is_arithmetic_v<
		                  std::remove_reference_t<T>>) {
			json += toString(std::forward<T>(val));
		} else {
			if constexpr (requires{ static_cast<bool>(val); *val; }) {
				if (!val) {
					json += "null";
					return;
				}
			}
			json += '\"';
			json += Text::SmartString::escape(
			    toString(std::forward<T>(val)));
			json += '\"';
		}
	}

	template <class T>
	inline auto operator()(T &&val,
	    const std::initializer_list<std::string_view> &il)
	    -> std::void_t<decltype(toString(std::forward<T>(val)))>
	{
		pre(il);
		primitive(std::forward<T>(val));
	}

	template <class T> inline void array(T &&val)
	{
		json += '[';
		bool not_first = false;
		for (const auto &e : val) {
			if (std::exchange(not_first, true)) json += ',';
			if constexpr (requires { toString(e); }) { primitive(e); }
			else if constexpr (std::ranges::range<decltype(e)>) {
				array(e);
			}
			else {
				Refl::visit(Json{json}, e);
			}
		}
		json += ']';
	}

	template <class T>
	inline auto operator()(T &&val,
	    const std::initializer_list<std::string_view> &il)
	    -> std::enable_if_t<std::ranges::range<T>>
	{
		pre(il);
		array(std::forward<T>(val));
	}

	Json(std::string &json) : json(json) { json += '{'; }

	~Json()
	{
		if (curr) json.append(std::size(*curr) - 1, '}');
		json += '}';
	}
	std::string &json;
	const std::initializer_list<std::string_view> *curr{};
};

template <class T> std::string toJson(T &&v)
{
	std::string res;
	Refl::visit(Json{res}, v);
	return res;
}
}

#endif
