#ifndef BASE_AUTO_JSON_H
#define BASE_AUTO_JSON_H

#include <initializer_list>
#include <optional>
#include <ranges>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "base/refl/auto_struct.h"
#include "base/text/smartstring.h"

#include "tostring.h"

namespace Conv
{

template <class T>
concept JSONSerializable = requires(const T &val) { val.toJSON(); };

template <class T>
concept Optional =
    !JSONSerializable<T> && !std::convertible_to<T, std::string>
    && requires(const T &val) {
	       static_cast<bool>(val);
	       *val;
       };

template <class T> concept Tuple = sizeof(std::tuple_size<T>) > 0;

template <class T>
concept Pair = Tuple<T> && std::tuple_size_v<T> == 2;

template <class T>
concept StringConvertable =
    !JSONSerializable<T> && !Optional<T>
    && requires(T &val) { ::Conv::toString(val); };

template <class T>
concept SerializableRange =
    !JSONSerializable<T> && !Optional<T> && !StringConvertable<T>
    && std::ranges::range<T>;

struct JSON
{
	template <class IL> inline void closeOpenObj(IL &&il)
	{
		auto from = std::begin(il);
		auto end = std::end(il);

		if (!std::empty(curr)) {
			auto [pre, cur] = std::ranges::mismatch(curr, il);
			if (auto cend = std::end(curr); pre != cend) [[likely]]
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
		else {
			json += "{";
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
		if constexpr (std::is_lvalue_reference_v<IL>) { curr = il; }
		else {
			curr = saved.emplace(std::forward<IL>(il));
		}
	}

	template <class T> inline void primitive(T &&val)
	{
		if constexpr (std::is_arithmetic_v<
		                  std::remove_reference_t<T>>) {
			json += toString(std::forward<T>(val));
		}
		else {
			json += '\"';
			json += Text::SmartString::escape(
			    toString(std::forward<T>(val)));
			json += '\"';
		}
	}

	template <class T> inline void array(T &&val)
	{
		json += '[';
		bool not_first = false;
		for (const auto &e : val) {
			if (std::exchange(not_first, true)) json += ',';
			any(e);
		}
		json += ']';
	}

	template <class T>
	inline __attribute__((always_inline)) void any(T &&val)
	{
		if constexpr (JSONSerializable<T>) { json += val.toJSON(); }
		else if constexpr (std::is_same_v<std::remove_cvref_t<T>,
		                       std::nullptr_t>
		                   || std::is_same_v<std::remove_cvref_t<T>,
		                       std::nullopt_t>) {
			json += "null";
		}
		else if constexpr (Optional<T>) {
			if (!val)
				json += "null";
			else
				any(*val);
		}
		else if constexpr (StringConvertable<T>) {
			primitive(std::forward<T>(val));
		}
		else if constexpr (SerializableRange<T>) {
			if constexpr (Pair<std::ranges::range_value_t<T>>) {
				if (std::empty(val)) { json += "{}"; }
				else {
					JSON j{json};
					bool which{};
					std::array<std::string, 2> strings;
					for (const auto &[k, v] : val) {
						j(v, {strings[which ^= true] = toString(k)});
					}
				}
			}
			else {
				array(std::forward<T>(val));
			}
		}
		else if constexpr (Tuple<T>) {
			std::apply(
			    [this](auto &&arg, auto &&...args)
			    {
				    json += '[';
				    any(std::forward<decltype(arg)>(arg));
				    ((json += ',',
				         any(std::forward<decltype(args)>(args))),
				        ...);
				    json += ']';
			    },
			    std::forward<T>(val));
		}
		else {
			Refl::visit(JSON{json}, val);
		}
	}

	template <class T>
	inline JSON &operator()(T &&val,
	    std::initializer_list<std::string_view> &&il)
	{
		closeOpenObj(
		    std::forward<std::initializer_list<std::string_view>>(
		        il));
		any(std::forward<T>(val));
		return *this;
	}

	template <class T>
	    requires(JSONSerializable<T> || Optional<T>
	             || StringConvertable<T> || SerializableRange<T>
	             || Tuple<T>)
	inline __attribute__((always_inline)) JSON &operator()(T &&val,
	    const std::initializer_list<std::string_view> &il)
	{
		closeOpenObj(il);
		any(std::forward<T>(val));
		return *this;
	}

	explicit inline JSON(std::string &json) : json(json) {}

	inline ~JSON()
	{
		if (!std::empty(curr)) json.append(std::size(curr), '}');
	}
	std::string &json;
	std::optional<std::vector<std::string_view>> saved;
	std::span<const std::string_view> curr;
};

template <class T> inline std::string toJSON(T &&v)
{
	std::string res;
	JSON{res}.any(std::forward<T>(v));
	return res;
}
}

#endif
