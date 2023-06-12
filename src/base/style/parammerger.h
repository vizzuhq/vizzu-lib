#ifndef STYLE_PARAMMERGER
#define STYLE_PARAMMERGER

#include <cstddef>
#include <functional>
#include <string>
#include <type_traits>

#include "base/refl/struct.h"

namespace Style
{

template <typename Root> struct ParamMerger
{
	std::byte *result;
	std::byte *other;
	Root merged;

	ParamMerger(const Root &base, Root &sub)
	{
		merged = base;
		result = reinterpret_cast<std::byte *>(&merged);
		other = reinterpret_cast<std::byte *>(&sub);
		merged.visit(*this);
	}

	template <typename T>
	ParamMerger &operator()(T &value, const char *)
	{
		if constexpr (Refl::isReflectable<T, ParamMerger>) {
			value.visit(*this);
		}
		else {
			const T &otherValue = *reinterpret_cast<const T *>(
			    other
			    + (reinterpret_cast<std::byte *>(&value) - result));
			if (otherValue) value = *otherValue;
		}
		return *this;
	}
};

}

#endif
