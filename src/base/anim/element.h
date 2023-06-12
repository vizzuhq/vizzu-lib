#ifndef ANIM_ELEMENT
#define ANIM_ELEMENT

#include "base/math/interpolation.h"

namespace Anim
{

struct IElement
{
	virtual ~IElement() {}
	virtual void transform(double factor) = 0;
};

template <typename T> class SingleElement : public IElement
{
public:
	SingleElement(const T &source, const T &target, T &actual) :
	    source(source),
	    target(target),
	    actual(actual)
	{}

	void transform(double factor) override
	{
		using namespace Math;
		actual = interpolate(source, target, factor);
	}

	const T &source;
	const T &target;
	T &actual;
};

}

#endif
