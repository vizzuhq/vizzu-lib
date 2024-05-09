#ifndef TYPE_BOOLITER_H
#define TYPE_BOOLITER_H

#include <array>

namespace Type
{
struct Bools
{
	constexpr static std::array boolArr = {false, true};
	bool max{true};
	[[nodiscard]] static const bool *begin()
	{
		return boolArr.data();
	}
	[[nodiscard]] const bool *end() const
	{
		return boolArr.data() + 1 + max;
	}
};
}

#endif
