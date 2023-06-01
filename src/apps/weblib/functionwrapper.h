#ifndef LIB_FUNCTIONWRAPPER_H
#define LIB_FUNCTIONWRAPPER_H

#include <memory>

#include "interfacejs.h"
//	extern void removeJsFunction(void *);

namespace Vizzu
{

template <class R, class T>
class FunctionWrapper
{
private:
	typedef R(*Fn)(const void *);
	typedef void(*Deleter)(Fn);

	struct Releaser {
		Releaser(Fn fn, Deleter deleter) : fn(fn), deleter(deleter) {}
		~Releaser() { deleter(fn); }
		Fn fn;
		Deleter deleter;
	};

public:
	static auto wrap(const Fn &fn)
	{
		if (!fn) return std::function<R(const T &)>();

		std::shared_ptr<Releaser> releaser;

		auto deleter = reinterpret_cast<void (*)(Fn)>(removeJsFunction);

		releaser = std::make_shared<Releaser>(fn, deleter);

		std::function<R(const T &)> func;

		func = [&, fn, releaser](const T &param) -> R
		{
			return fn(static_cast<const void *>(&param));
		};

		return func;
	}
};

}

#endif
