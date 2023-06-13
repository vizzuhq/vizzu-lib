#ifndef REFL_ENUM
#define REFL_ENUM

#include <algorithm>
#include <array>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>

#include "base/text/stringalg.h"

namespace Refl
{

// Note: extracted from template to reduce binary size
struct EnumReflectorBase
{
	static constexpr std::string_view name(std::size_t value,
	    const std::string_view *names,
	    size_t size)
	{
		if (value < size) return names[value];
		throw std::logic_error(
		    "not an enum value: '" + std::to_string(value)
		    + "', valid values: 0.." + std::to_string(size - 1));
	}

	static constexpr std::size_t value(std::string_view name,
	    const std::string_view *names,
	    size_t size,
	    std::string_view code)
	{
		for (auto i = 0u; i < size; i++)
			if (name == names[i]) return i;
		throw std::logic_error(
		    "not an enum name: '" + std::string(name)
		    + "', valid name: " + std::string(code));
	}
};

template <class EnumDefinition>
class EnumReflector : private EnumReflectorBase
{
public:
	constexpr EnumReflector() {}

	static consteval std::size_t count()
	{
		auto code = EnumDefinition::code;
		return std::count(code.begin(), code.end(), ',') + 1;
	}

	static constexpr std::string_view name(std::size_t value)
	{
		return EnumReflectorBase::name(value,
		    &names[0],
		    names.size());
	}

	static constexpr std::size_t value(std::string_view name)
	{
		return EnumReflectorBase::value(name,
		    &names[0],
		    names.size(),
		    EnumDefinition::code);
	}

	static constexpr auto getNames()
	{
		std::array<std::string_view, count()> names;
		auto code = EnumDefinition::code;
		for (auto i = 0u; i < names.size(); i++) {
			auto name = Text::String::popTill(code, ',');
			code.remove_prefix(name.size());
			name = Text::String::chop(name, ',');
			name = Text::String::trim(name);
			names[i] = name;
		}
		return names;
	}

	static constexpr std::array<std::string_view, count()> names =
	    getNames();
};

}

#define refEnumBeg(EnumName)                                         \
	EnumName                                                         \
	{                                                                \
	public:                                                          \
		enum EnumType : uint32_t;                                    \
		struct EnumDefinition;                                       \
                                                                     \
		typedef Refl::EnumReflector<EnumDefinition> EnumInfo;        \
                                                                     \
		constexpr EnumName() {}                                      \
		constexpr EnumName(EnumType value) : value(value) {}         \
		explicit constexpr EnumName(uint32_t v) : value(EnumType(v)) \
		{}                                                           \
                                                                     \
		explicit constexpr EnumName(std::string_view name)           \
		{                                                            \
			value = static_cast<EnumType>(EnumInfo::value(name));    \
		}                                                            \
                                                                     \
		constexpr operator EnumType() const { return value; }        \
		explicit constexpr operator uint32_t() const                 \
		{                                                            \
			return value;                                            \
		}                                                            \
		explicit constexpr operator std::string_view() const         \
		{                                                            \
			return EnumInfo::name(value);                            \
		}                                                            \
                                                                     \
		explicit operator std::string() const                        \
		{                                                            \
			return std::string(EnumInfo::name(value));               \
		}                                                            \
                                                                     \
		EnumType value;

#define refEnumMid(...) enum EnumType : uint32_t { __VA_ARGS__ };

#define refEnumEnd(...)                                       \
	struct EnumDefinition                                     \
	{                                                         \
		static constexpr std::string_view code = __VA_ARGS__; \
	};                                                        \
	}

#define refEnumSecondPart(...) \
	refEnumMid(__VA_ARGS__) refEnumEnd(#__VA_ARGS__)

#define Enum(EnumName) refEnumBeg(EnumName) refEnumSecondPart

#define refEnumSecondPartNoNames(...) \
	refEnumMid(__VA_ARGS__) refEnumEnd

#define SpecNameEnum(EnumName) \
	refEnumBeg(EnumName) refEnumSecondPartNoNames

#endif
