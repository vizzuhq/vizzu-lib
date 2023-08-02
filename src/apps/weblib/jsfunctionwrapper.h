#ifndef LIB_JSFUNCTIONWRAPPER_H
#define LIB_JSFUNCTIONWRAPPER_H

#include <memory>

#include "interfacejs.h"

namespace Vizzu
{

template <class R, class... Ts> class JsFunctionWrapper
{
private:
	using JsFun = R(std::conditional_t<
	    std::is_const_v<std::remove_reference_t<Ts>>,
	    const void *,
	    void *>...);

public:
	constexpr JsFunctionWrapper(std::shared_ptr<JsFun> &&fun = {}) :
	    wrapper{std::move(fun)}
	{}

	operator std::function<R(Ts...)>() &&
	{
		if (wrapper.fun)
			return {std::move(wrapper)};
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
