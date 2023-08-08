#ifndef BASE_AUTO_JSON_H
#define BASE_AUTO_JSON_H

#include <cassert>
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

template <class T> concept Tuple = sizeof(std::tuple_size<T>) != 0;

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
	template <class T> inline void primitive(const T &val) const
	{
		if constexpr (std::is_arithmetic_v<T>) {
			json += toString(val);
		}
		else if constexpr (std::is_enum_v<T>
		                   || std::is_same_v<T, bool>) {
			json += '\"';
			json += toString(val);
			json += '\"';
		}
		else {
			json += '\"';
			json += Text::SmartString::escape(toString(val));
			json += '\"';
		}
	}

	template <class T> inline void array(const T &val) const
	{
		json += '[';
		bool not_first = false;
		for (const auto &e : val) {
			if (not_first)
				json += ',';
			else
				not_first = true;
			any(e);
		}
		json += ']';
	}

	template <class T> inline void tupleObj(const T &val) const
	{
		std::apply(
		    [this](const auto &arg, const auto &...args)
		    {
			    json += '[';
			    any(arg);
			    ((json += ',', any(args)), ...);
			    json += ']';
		    },
		    val);
	}

	template <class T> void dynamicObj(const T &val) const;

	template <class T> void staticObj(const T &val) const;

	template <class T> inline void any(const T &val) const
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
			primitive(val);
		}
		else if constexpr (SerializableRange<T>) {
			if constexpr (Pair<std::ranges::range_value_t<T>>) {
				dynamicObj(val);
			}
			else {
				array(val);
			}
		}
		else if constexpr (Tuple<T>) {
			tupleObj(val);
		}
		else {
			staticObj(val);
		}
	}

	explicit inline JSON(std::string &json) : json(json) {}

	std::string &json;
};

struct JSONAutoObj : JSON
{
	using JSON::JSON;

	inline ~JSONAutoObj()
	{
		if (cp)
			json.append(std::size(*cp), '}');
		else
			json += "{}";
	}

	inline void closeOpenObj(
	    const std::initializer_list<std::string_view> &il)
	{
		const auto *from = std::begin(il);
		const auto *end = std::end(il);

		if (cp) {
			auto [pre, cur] = std::ranges::mismatch(*cp, il);
			assert(pre != std::end(*cp));
			json.append(std::end(*cp) - pre - 1, '}');
			json += ',';
			from = cur;
		}
		else
			json += '{';

		assert(from != end);
		while (true) {
			json += '\"';
			json.append(*from);
			json += "\":";
			if (++from != end)
				json += '{';
			else
				break;
		}
		cp = &il;
	}

	template <class T>
	    requires(JSONSerializable<T> || Optional<T>
	             || StringConvertable<T> || SerializableRange<T>
	             || Tuple<T>)
	inline void operator()(const T &val,
	    const std::initializer_list<std::string_view> &il)
	{
		closeOpenObj(il);
		any(val);
	}

	template <class T>
	inline void operator()(const T &val,
	    std::initializer_list<std::string_view> &&il) = delete;

	const std::initializer_list<std::string_view> *cp{};
};

struct JSONObj : JSON
{
	using JSON::JSON;

	inline ~JSONObj()
	{
		if (!was) json += '{';
		json += '}';
	}

	template <bool KeyNoEscape = true>
	inline void key(std::string_view key)
	{
		json += std::exchange(was, true) ? ',' : '{';

		json += '\"';
		if constexpr (KeyNoEscape) { json.append(key); }
		else {
			json += Text::SmartString::escape(std::string{key});
		}
		json += "\":";
	}

	template <bool KeyNoEscape = true>
	inline JSONObj nested(std::string_view key)
	{
		this->key<KeyNoEscape>(key);
		return JSONObj{json};
	}

	template <bool KeyNoEscape = true>
	inline JSONObj &raw(std::string_view key, const std::string &str)
	{
		this->key<KeyNoEscape>(key);
		json += str;
		return *this;
	}

	template <bool KeyNoEscape = true, class T>
	inline JSONObj &operator()(std::string_view key, const T &val)
	{
		this->key<KeyNoEscape>(key);
		any(val);
		return *this;
	}

	bool was{};
};

template <class T> inline void JSON::dynamicObj(const T &val) const
{
	JSONObj j{json}; // NOLINT
	for (const auto &[k, v] : val) { j(toString(k), v); }
}

template <class T> inline void JSON::staticObj(const T &val) const
{
	Refl::visit(JSONAutoObj{json}, val);
}

template <class T> inline std::string toJSON(const T &v)
{
	std::string res;
	JSON{res}.any(v);
	return res;
}
}

#endif
