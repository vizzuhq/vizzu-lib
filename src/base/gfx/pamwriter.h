#ifndef GFX_PAMWRITER
#define GFX_PAMWRITER

#include <iostream>

#include "pixmap.h"

namespace Gfx
{

class PamWriter
{
public:
	PamWriter(std::ostream &stream);
	void write(const PixMapView &pixmap);

private:
	std::ostream &stream;
};

}

#endif
