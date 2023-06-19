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
	constexpr JsFunctionWrapper(JsFun fun) : fun(fun)
	{
		if (fun)
			releaser = {reinterpret_cast<void (*)()>(fun),
			    removeJsFunction};
	}

	operator std::function<R(Ts...)>() const &&
	{
		if (fun)
			return {std::move(*this)};
		else
			return {};
	}

	R operator()(Ts &&...ts) const { return fun(&ts...); }

	JsFun fun;
private:
	std::shared_ptr<void()> releaser;
};

}

#endif
