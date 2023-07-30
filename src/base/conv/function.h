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

using Params = std::vector<std::string>;

template <class C>
using FunctionType = std::function<std::string(C &, const Params &)>;

template <typename Sequence, typename MT,
    typename R, class C, typename... P>
struct Functor;

template <typename R, typename MT,
    class C, typename... P, std::size_t... Ix>
struct Functor<std::index_sequence<Ix...>, MT, R, C, P...>
{
	MT method;

	std::string operator()(C &obj, const Params &params)
	{
		if (params.size() != sizeof...(P))
			throw std::logic_error("invalid parameter count");

		std::string result;

		if constexpr (std::is_same_v<R, void>)
			(obj.*method)(parse<P>(params[Ix])...);
		else
			result = toString((obj.*method)(parse<P>(params[Ix])...));

		return result;
	}
};

template <typename R, class C, typename... P>
Functor(R (C::*method)(P...))
    -> Functor<std::index_sequence_for<P...>,
        R (C::*)(P...),
        R, C, P...>;

template <typename R, class C, typename... P>
Functor(R (C::*method)(P...) const)
    -> Functor<std::index_sequence_for<P...>,
        R (C::*)(P...) const,
        const R, C, P...>;

template <typename R, class C, typename... P>
FunctionType<C> function(R (C::*method)(P...))
{
	return Functor(method);
}

template <typename R, class C, typename... P>
FunctionType<C> function(R (C::*method)(P...) const)
{
	return Functor(method);
}

}

#endif
