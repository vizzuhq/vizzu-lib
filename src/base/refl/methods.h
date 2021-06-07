#ifndef REFL_METHODS
#define REFL_METHODS

#include <map>

#include "base/conv/function.h"

namespace Refl
{

template <class Sub>
class Methods
{
public:
	typedef Conv::FunctionType<Sub> Method;

	std::string call(const std::string &name, const Conv::Params &params)
	{
		auto it = methods.find(name);
		if (it != methods.end()) {
			auto &sub = static_cast<Sub &>(*this);
			return it->second(sub, params);
		}
		else throw std::logic_error("invalid method name");
	}

private:
	typedef std::map<std::string, Method> MethodMap;

	static MethodMap methods;

	struct MethodVisitor {
		MethodMap res;
		auto &operator()(auto method, const char *name)
		{
			res.emplace(name, Conv::function(method));
			return *this;
		}
	};

	static MethodMap initMethods()
	{
		MethodVisitor visitor;
		Sub::reflect(visitor);
		return visitor.res;
	}
};

template <class Sub>
typename Methods<Sub>::MethodMap
Methods<Sub>::methods = Methods<Sub>::initMethods();

}

#endif
