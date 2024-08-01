#ifndef TYPE_PHYSICALVALUE
#define TYPE_PHYSICALVALUE

#include <cstdint>

namespace Type
{

enum class SimpleUnit : std::uint8_t { none, relative, absolute };

template <typename Value, typename Unit = SimpleUnit>
class PhysicalValue
{
public:
	Value value;
	Unit unit;

	constexpr PhysicalValue() : value{}, unit{} {}
	constexpr PhysicalValue(Value value, Unit unit) :
	    value(value),
	    unit(unit)
	{}

	constexpr bool operator==(const PhysicalValue &) const = default;
};

}

#endif
