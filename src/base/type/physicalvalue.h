#ifndef TYPE_PHYSICALVALUE
#define TYPE_PHYSICALVALUE

#include <cstdint>

namespace Type
{

enum class SimpleUnit : uint32_t {
	none,
	relative,
	absolute
};

template <typename Value, typename Unit = SimpleUnit>
class PhysicalValue
{
public:
	Value value;
	Unit unit;

	PhysicalValue() : value(Value()), unit(Unit(0)) {}
	PhysicalValue(Value value, Unit unit) : value(value), unit(unit) {}

	bool operator==(const auto &other) const {
		return value == other.value
			&& unit == other.unit;
	}
};

}

#endif
