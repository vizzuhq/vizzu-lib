#ifndef LIB_JSFUNCTIONWRAPPER_H
#define LIB_JSFUNCTIONWRAPPER_H

#include <memory>

#include "interfacejs.h"

namespace Vizzu
{

template <class R, class... Ts> class JsFunctionWrapper
{
private:
	using JsFun = R (*)(std::conditional_t<
	    std::is_const_v<std::remove_reference_t<Ts>>,
	    const void *,
	    void *>...);

public:
	constexpr JsFunctionWrapper(JsFun fun) : wrapper{fun, {}}
	{
		if (fun)
			wrapper.releaser = {reinterpret_cast<void (*)()>(fun),
			    removeJsFunction};
	}

	operator std::function<R(Ts...)>() &&
	{
		if (wrapper.fun)
			return {std::move(wrapper)};
		else
			return {};
	}

	std::size_t hash() const noexcept
	{
		return std::hash<JsFun>{}(wrapper.fun);
	}

private:
	struct
	{
		JsFun fun;
		std::shared_ptr<void()> releaser;
		R operator()(Ts &&...ts) const { return fun(&ts...); }
	} wrapper;
};

}

#endif
