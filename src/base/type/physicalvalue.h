#ifndef TYPE_PHYSICALVALUE
#define TYPE_PHYSICALVALUE

#include "base/conv/parse.h"
#include "base/conv/tostring.h"
#include "base/refl/auto_enum.h"
#include "base/text/valueunit.h"

namespace Type
{

template <typename Value, Refl::is_enum Unit> struct PhysicalValue
{
	Value value{};
	Unit unit{};

	constexpr bool operator==(const PhysicalValue &) const = default;

	template <std::same_as<double> = Value>
	[[nodiscard]] static PhysicalValue fromString(
	    const std::string &str)
	{
		const Text::ValueUnit vu{str};
		return {vu.getValue(), Refl::get_enum<Unit>(vu.getUnit())};
	}

	[[nodiscard]] std::string toString() const
	{
		return Conv::toString(value)
		     + std::string{Conv::toString(unit)};
	}
};

}

#endif
