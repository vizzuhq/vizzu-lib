#ifndef GFX_SVG
#define GFX_SVG

#include <string>

namespace Gfx
{

class Svg
{
public:
	Svg(const std::string &data) : data(data) {}
	std::string data;
};

}

#endif
