#ifndef TYPE_BOOLITER_H
#define TYPE_BOOLITER_H

#include <array>

namespace Type
{
template <class T = bool>
    requires(std::is_same_v<T, bool>
             || std::is_same_v<std::underlying_type_t<T>, bool>)
struct Bools
{
	constexpr static std::array boolArr = {static_cast<T>(false),
	    static_cast<T>(true)};
	T max{boolArr[1]};
	[[nodiscard]] static const T *begin() { return boolArr.data(); }
	[[nodiscard]] const T *end() const
	{
		return boolArr.data() + 1 + static_cast<bool>(max);
	}
};

template <class T> Bools(T) -> Bools<T>;

}

#endif
