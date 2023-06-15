#ifndef LIB_FUNCTIONWRAPPER_H
#define LIB_FUNCTIONWRAPPER_H

#include <memory>

#include "interfacejs.h"

namespace Vizzu
{

template <class R, class T> class FunctionWrapper
{
private:
	typedef R (*Fn)(const void *);

	struct Releaser
	{
		Releaser(Fn fn) : fn(fn) {}
		~Releaser() { removeJsFunction(static_cast<void *>(fn)); }
		Fn fn;
	};

public:
	static std::function<R(const T &)> wrap(const Fn &fn)
	{
		if (!fn) return std::function<R(const T &)>();
		return [fn, releaser = std::make_shared<Releaser>(fn)](
		           const T &param) -> R
		{
			return fn(static_cast<const void *>(&param));
		};
	}
};

}

#endif
