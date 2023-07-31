#ifndef ALIGN_H
#define ALIGN_H

#include <string>

#include "base/math/range.h"

namespace Vizzu::Base
{

struct Align : private Math::Range<double>
{
public:
	enum class Type : uint32_t { none, min, center, max, stretch };

	Align(Type type, const Range<double> &range) :
	    Range<double>(range),
	    type(type)
	{}

	Range<double> getAligned(const Range<double> &range)
	{
		switch (type) {
		case Type::none: return range;
		case Type::min: return range + min - range.getMin();
		case Type::center: return range - range.middle() + middle();
		case Type::max: return range + max - range.getMax();
		case Type::stretch: return *this;
		}
		throw std::logic_error("invalid align type");
	}

private:
	Type type;
};

}

#endif
