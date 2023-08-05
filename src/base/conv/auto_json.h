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
	template <class T> inline void primitive(const T &val) const
	{
		if constexpr (std::is_arithmetic_v<
		                  std::remove_reference_t<T>>) {
			json += toString(val);
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
		if (curr.data() != nullptr)
			json.append(std::size(curr), '}');
		else
			json += "{}";
	}

	template <bool Trusted = true>
	inline void closeOpenObj(
	    const std::initializer_list<std::string_view> &il) const
	{
		auto from = std::begin(il);
		auto end = std::end(il);

		if (curr.data() != nullptr) {
			auto [pre, cur] = std::ranges::mismatch(curr, il);
			if (auto cend = std::end(curr); pre != cend) [[likely]]
				json.append(cend - pre - 1, '}');
			json += ',';
			from = cur;
		}
		else
			json += '{';

		if (from != end) [[likely]] {
			while (true) {
				json += '\"';
				if constexpr (Trusted) { json.append(*from); }
				else {
					json +=
					    Text::SmartString::escape(std::string{*from});
				}
				json += "\":";
				if (++from != end)
					json += '{';
				else
					break;
			}
		}
	}

	template <class T>
	    requires(JSONSerializable<T> || Optional<T>
	             || StringConvertable<T> || SerializableRange<T>
	             || Tuple<T>)
	inline JSONAutoObj &operator()(const T &val,
	    const std::initializer_list<std::string_view> &il)
	{
		closeOpenObj(il);
		curr = il;
		any(val);
		return *this;
	}

	template <class T>
	inline JSONAutoObj &operator()(const T &val,
	    std::initializer_list<std::string_view> &&il) = delete;

	std::span<const std::string_view> curr;
};

struct JSONObj : JSONAutoObj
{
	using JSONAutoObj::JSONAutoObj;

	template <bool Trusted = true, class T>
	inline JSONObj &operator()(const T &val,
	    std::initializer_list<std::string_view> &&il)
	{
		closeOpenObj<Trusted>(il);
		curr = saved.emplace(il);
		any(val);
		return *this;
	}

	std::optional<std::vector<std::string_view>> saved;
};

template <class T> inline void JSON::dynamicObj(const T &val) const
{
	JSONObj j{json};
	bool which{};
	std::array<std::string, 2> strings;
	for (const auto &[k, v] : val) {
		j(v, {strings[which ^= true] = toString(k)});
	}
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
