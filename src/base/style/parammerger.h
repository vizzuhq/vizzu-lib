#ifndef STYLE_PARAMMERGER
#define STYLE_PARAMMERGER

#include <cstddef>
#include <functional>
#include <string>
#include <type_traits>

#include "base/refl/auto_struct.h"

#include "param.h"

namespace Style
{

struct ParamMerger
{
	template <IsParam T> void operator()(T &base, T const &sub)
	{
		if (sub) base = sub;
	}

	template <class T>
	static T merge(std::type_identity_t<T> base, T const &sub)
	{
		Refl::visit(ParamMerger{}, base, sub);
		return base;
	}
};

}

#endif
