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
#include "base/text/character.h"
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
    && requires(const T &val) { ::Conv::toString(val); };

template <class T>
concept SerializableRange =
    !JSONSerializable<T> && !Optional<T> && !StringConvertable<T>
    && std::ranges::range<T>;

struct JSON
{
	template <class T> inline void escaped(const T &str) const
	{
		for (auto ch : str) {
			if (ch >= 0 && ch <= 31) [[unlikely]] {
				json += "\\u00";
				using Text::Character::toHex;
				json += toHex(ch >= 16);
				json += toHex(ch % 16);
				continue;
			}
			else if (ch == '\\' || ch == '"')
				json += '\\';

			json += ch;
		}
	}

	template <class T> inline void primitive(const T &val) const
	{
		if constexpr (std::is_floating_point_v<T>) {
			if (std::isfinite(val))
				json += toString(val);
			else
				json += "null";
		}
		else if constexpr (std::is_arithmetic_v<T>) {
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
			if constexpr (std::is_same_v<T, std::string_view>)
				escaped(val);
			else
				escaped(toString(val));

			json += '\"';
		}
	}

	template <class T> inline void array(const T &val) const;

	template <class T> inline void tupleObj(const T &val) const;

	template <class T> void dynamicObj(const T &val) const;

	template <class T> void staticObj(const T &val) const;

	template <class T>
	    requires(JSONSerializable<T>)
	inline void any(const T &val) const
	{
		json += val.toJSON();
	}

	template <class T>
	    requires(
	        std::is_same_v<std::remove_cvref_t<T>, std::nullptr_t>
	        || std::is_same_v<std::remove_cvref_t<T>, std::nullopt_t>)
	inline void any(const T &) const
	{
		json += "null";
	}

	template <class T>
	    requires(Optional<T>)
	inline void any(const T &val) const
	{
		if (!val)
			json += "null";
		else
			any(*val);
	}

	template <class T>
	    requires(StringConvertable<T>)
	inline void any(const T &val) const
	{
		primitive(val);
	}

	template <class T>
	    requires(SerializableRange<T>)
	inline void any(const T &val) const
	{
		if constexpr (Pair<std::ranges::range_value_t<T>>) {
			dynamicObj(val);
		}
		else {
			array(val);
		}
	}
	template <class T>
	    requires(
	        !JSONSerializable<T> && !SerializableRange<T> && Tuple<T>)
	inline void any(const T &val) const
	{
		tupleObj(val);
	}

	template <class T>
	    requires(
	        !JSONSerializable<T>
	        && !std::is_same_v<std::remove_cvref_t<T>, std::nullptr_t>
	        && !std::is_same_v<std::remove_cvref_t<T>, std::nullopt_t>
	        && !Optional<T> && !StringConvertable<T>
	        && !SerializableRange<T> && !Tuple<T>)
	inline void any(const T &val) const
	{
		staticObj(val);
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

	JSONAutoObj(JSONAutoObj &&) = delete;
	JSONAutoObj(const JSONAutoObj &) = delete;
	JSONAutoObj &operator=(JSONAutoObj &&) = delete;
	JSONAutoObj &operator=(const JSONAutoObj &) = delete;

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
		do {
			json += '\"';
			json.append(*from);
			json += "\":";
		}
		while ((++from != end) && (json += '{', true));
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

template <class J> struct JSONNoBaseAutoObj : JSONAutoObj
{
	using JSONAutoObj::JSONAutoObj;

	template <class T>
	    requires(!std::is_base_of_v<J, T>)
	inline auto operator()(const T &val,
	    const std::initializer_list<std::string_view> &il)
	    -> decltype(std::declval<JSONAutoObj &>()(val, il))
	{
		return static_cast<JSONAutoObj &>(*this)(val, il);
	}
};

template <char open, char close> struct JSONRepeat : JSON
{
	using JSON::JSON;

	JSONRepeat(JSONRepeat &&) = delete;
	JSONRepeat(const JSONRepeat &) = delete;
	JSONRepeat &operator=(const JSONRepeat &) = delete;
	JSONRepeat &operator=(JSONRepeat &&) = delete;

	inline ~JSONRepeat()
	{
		if (!was) json += open;
		json += close;
	}

	void sep() { json += std::exchange(was, true) ? ',' : open; }

	bool was{};
};

struct JSONArr;

struct JSONObj : protected JSONRepeat<'{', '}'>
{
	using JSONRepeat::JSONRepeat;

	template <bool KeyNoEscape = true>
	inline JSON &key(std::string_view key)
	{
		sep();
		json += '\"';
		if constexpr (KeyNoEscape)
			json.append(key);
		else
			escaped(key);

		json += "\":";
		return *this;
	}

	template <bool KeyNoEscape = true>
	inline JSONObj nested(std::string_view key)
	{
		this->key<KeyNoEscape>(key);
		return JSONObj{json};
	}

	template <bool KeyNoEscape = true>
	inline JSONArr nestedArr(std::string_view key);

	template <bool KeyNoEscape = true>
	inline JSONObj &raw(std::string_view key, const std::string &str)
	{
		this->key<KeyNoEscape>(key);
		json += str;
		return *this;
	}

	template <bool KeyNoEscape = true, class T>
	inline JSONObj &operator()(std::string_view key, const T &val) &
	{
		this->key<KeyNoEscape>(key);
		any(val);
		return *this;
	}

	template <bool KeyNoEscape = true, class T>
	inline JSONObj &&operator()(std::string_view key, const T &val) &&
	{
		this->key<KeyNoEscape>(key);
		any(val);
		return std::move(*this);
	}

	inline JSONObj &&merge(std::string_view jsonObj) &&
	{
		if (jsonObj.size() > 2) {
			sep();
			json += jsonObj.substr(1, jsonObj.size() - 2);
		}
		return std::move(*this);
	}
};

struct JSONArr : protected JSONRepeat<'[', ']'>
{
	using JSONRepeat::JSONRepeat;

	template <class T> inline JSONArr &operator<<(const T &obj)
	{
		sep();
		any(obj);
		return *this;
	}

	inline JSONArr nested()
	{
		sep();
		return JSONArr{json};
	}

	inline JSONObj nestedObj()
	{
		sep();
		return JSONObj{json};
	}
};

template <class T> inline void JSON::array(const T &val) const
{
	auto arr = JSONArr{json};
	for (const auto &e : val) arr << e;
}

template <class T> inline void JSON::tupleObj(const T &val) const
{
	std::apply(
	    [this](const auto &...args)
	    {
		    (JSONArr{json} << ... << args);
	    },
	    val);
}

template <class T> inline void JSON::dynamicObj(const T &val) const
{
	auto j = JSONObj{json};
	for (const auto &[k, v] : val) {
		j.template operator()<false>(toString(k), v);
	}
}

template <class T> inline void JSON::staticObj(const T &val) const
{
	Refl::visit(JSONNoBaseAutoObj<T>{json}, val);
}

template <bool KeyNoEscape>
inline JSONArr JSONObj::nestedArr(std::string_view key)
{
	this->key<KeyNoEscape>(key);
	return JSONArr{json};
}

template <class T> inline std::string toJSON(const T &v)
{
	std::string res;
	JSON{res}.any(v);
	return res;
}
}

#endif
