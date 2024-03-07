#ifndef LIB_JSFUNCTIONWRAPPER_H
#define LIB_JSFUNCTIONWRAPPER_H

#include <memory>

#include "interfacejs.h"

namespace Vizzu
{

template <class R, class... Ts> class JsFunctionWrapper
{
	using JsFun = R(std::remove_reference_t<Ts> *...);

public:
	constexpr JsFunctionWrapper() noexcept = default;

	constexpr explicit JsFunctionWrapper(
	    std::shared_ptr<JsFun> &&fun) noexcept :
	    wrapper{std::move(fun)}
	{}

	explicit operator std::function<R(Ts...)>() &&
	{
		if (wrapper.fun) return {std::move(wrapper)};
		return {};
	}

	[[nodiscard]] std::size_t hash() const noexcept
	{
		return std::hash<std::shared_ptr<JsFun>>{}(wrapper.fun);
	}

private:
	struct
	{
		std::shared_ptr<JsFun> fun;
		R operator()(Ts &&...ts) const { return (*fun)(&ts...); }
	} wrapper;
};

}

#endif
