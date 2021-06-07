#ifndef ALIGN_H
#define ALIGN_H

#include <string>

#include "base/math/range.h"

namespace Vizzu
{
namespace Base
{

struct Align : private Math::Range<double>
{
public:
	enum Type : uint32_t { None, Min, Center, Max, Fit };

	Align(Type type, const Range<double> &range)
		: Range<double>(range), type(type)
	{}

	Range<double> getAligned(const Range<double> &range) {
		switch(type) {
		case None: return range;
		case Min: return range + min - range.getMin();
		case Center: return range - range.middle() + middle();
		case Max: return range + max - range.getMax();
		case Fit: return *this;
		}
		throw std::logic_error("invalid align type");
	}

private:
	Type type;
};

inline std::string toString(Align::Type type) {
	typedef Align::Type T;
	if (type == T::None) return "none";
	else if (type == T::Min) return "min";
	else if (type == T::Center) return "center";
	else if (type == T::Max) return "max";
	else if (type == T::Fit) return "fit";
	else return "unknown";
}


}
}

#endif

