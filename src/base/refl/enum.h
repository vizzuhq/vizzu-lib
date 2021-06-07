#ifndef REFL_ENUM
#define REFL_ENUM

#include <array>
#include <cstdint>
#include <stdexcept>
#include <string_view>
#include <string>

#include "base/text/stringalg.h"

namespace Refl
{

template <class EnumDefinition>
class EnumReflector
{
public:
	constexpr EnumReflector() {}

	static consteval std::size_t count() {
		auto code = EnumDefinition::code;
		return std::count(code.begin(), code.end(), ',') + 1;
	}

	static constexpr std::string_view name(std::size_t value)
	{
		if (value < names.size()) return names[value];
		throw std::logic_error(
		    "not an enum value: '" + std::to_string(value)
		    + "', valid values: 0.." + std::to_string(names.size()-1));
	}

	static constexpr std::size_t value(std::string_view name)
	{
		for (auto i = 0u; i < names.size(); i++)
			if (name == names[i]) return i;
		throw std::logic_error(
		    "not an enum name: '" + std::string(name)
		    + "', valid name: " + std::string(EnumDefinition::code));
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

	static constexpr std::array<std::string_view, count()> names = getNames();
};

}

#define Enum(EnumName)                                        \
	EnumName                                                  \
	{                                                         \
	public:                                                   \
		enum EnumType : uint32_t;                             \
		struct EnumDefinition;                                \
                                                              \
		typedef Refl::EnumReflector<EnumDefinition> EnumInfo; \
                                                              \
		EnumName() {}                                         \
		EnumName(EnumType value) : value(value) {}            \
                                                              \
		explicit EnumName(const std::string &name)            \
		{                                                     \
			value = (EnumType)EnumInfo::value(name);          \
		}                                                     \
                                                              \
		operator EnumType() const { return value; }           \
                                                              \
		explicit operator std::string() const                 \
		{                                                     \
			return std::string(EnumInfo::name(value));        \
		}                                                     \
                                                              \
		EnumType value;                                       \
                                                              \
		refEnumSpec

#define refEnumSpec(...)                                           \
		enum EnumType : uint32_t                                   \
		{                                                          \
			__VA_ARGS__                                            \
		};                                                         \
	                                                               \
		struct EnumDefinition                                      \
		{                                                          \
			static constexpr std::string_view code = #__VA_ARGS__; \
		};                                                         \
	}

#endif
