#ifndef GFX_PAMREADER
#define GFX_PAMREADER

#include <cstddef>
#include <istream>

#include "pixmap.h"

namespace Gfx
{

class PamReader
{
public:
	PamReader() {}

	static PixMapView parse(char *data, size_t size);
	static PixMap parse(std::istream &in);

private:
	struct ParsedHeader
	{
		PixMapView pixmap;
		size_t headerSize;
		ParsedHeader(std::istream &in);
	};
};

}

#endif
