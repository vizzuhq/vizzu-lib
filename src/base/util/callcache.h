#ifndef UTIL_CALLCACHE
#define UTIL_CALLCACHE

#include <tuple>

namespace Util
{

template <typename> class CallCache;

template <typename Res, class Class, typename... Params>
class CallCache<Res (Class::*)(Params...)>
{
public:
	typedef Res (Class::*WrappedFunc)(Params...);

	CallCache(WrappedFunc func) :
		f(func),
		lastParams(std::remove_cv_t<std::remove_reference_t<Params>>()...)
	{}

	typedef std::tuple<std::remove_cv_t<std::remove_reference_t<Params>>...> ParamsTuple;

	Res operator()(Class *obj, Params... params)
	{
		auto actParams = std::make_tuple(params...);
		if (lastObj != obj || actParams != lastParams)
		{
			auto res = (obj->*f)(params...);
			lastObj = obj;
			lastParams = actParams;
			lastRes = res;
			return res;
		}
		else
		{
			return lastRes;
		}
	}

private:
	Res (Class::*f)(Params...);
	Class *lastObj;
	ParamsTuple lastParams;
	Res lastRes;
};

}

#endif
