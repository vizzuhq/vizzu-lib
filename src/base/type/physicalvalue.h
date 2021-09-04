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
};

template <typename Value, typename Unit = SimpleUnit>
static bool operator==(
	const PhysicalValue<Value,Unit> &op1,
	const PhysicalValue<Value,Unit> &op2) 
{
	return op1.value == op2.value
		&& op1.unit == op2.unit;
}

}

#endif
