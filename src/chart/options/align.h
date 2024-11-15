#ifndef ALIGN_H
#define ALIGN_H

#include <string>

#include "base/math/range.h"

namespace Vizzu::Base
{

struct Align : private Math::Range<>
{
	enum class Type : std::uint8_t { none, center, stretch };

	Align(Type type, const Range &range) : Range(range), type(type) {}

	[[nodiscard]] Range getAligned(const Range &range) const
	{
		switch (type) {
		case Type::none: return range - range.getMin() + min;
		case Type::center: return range - range.middle() + middle();
		case Type::stretch: return static_cast<const Range &>(*this);
		}
		throw std::logic_error("invalid align type");
	}

private:
	Type type;
};

}

#endif
