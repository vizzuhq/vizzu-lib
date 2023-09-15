#ifndef COLORBASE_H
#define COLORBASE_H

#include <cstdint>

namespace Vizzu::Gen
{

struct ColorBase
{
	bool discrete{true};
	uint32_t index{0};
	double pos{0};
	double lightness{0};

	ColorBase() = default;
	ColorBase(uint32_t index, double lightness) :
	    index(index),
	    lightness(lightness)
	{}
	ColorBase(double pos, double lightness) :
	    discrete(false),
	    pos(pos),
	    lightness(lightness)
	{}

	bool operator==(const ColorBase &other) const = default;
};

}

#endif
