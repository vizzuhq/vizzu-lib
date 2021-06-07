#ifndef CONV_FUNCTION
#define CONV_FUNCTION

#include <functional>
#include <string>
#include <type_traits>
#include <vector>

#include "parse.h"
#include "tostring.h"

namespace Conv
{

typedef std::vector<std::string> Params;

template <class C>
using FunctionType =
	std::function<std::string(C&, const Params &)>;

template <typename R, class C, typename... P>
struct Functor
{
	R (C::*method)(P...);

	Functor(R (C::*method)(P...)) : method(method) {}

	std::string operator()(C &obj, const Params &params)
	{
		if (params.size() != sizeof...(P))
			throw std::logic_error("invalid parameter count");

		std::string result;
		auto it = params.begin();

		if constexpr (std::is_same_v<R, void>)
			(obj.*method)(parse<P>(*(it++))...);
		else
			result = toString((obj.*method)(parse<P>(*(it++))...));

		return result;
	}
};

template <typename R, class C, typename... P>
FunctionType<C> function(R (C::*method)(P...))
{
	return Functor(method);
}

}

#endif
