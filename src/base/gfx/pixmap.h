#ifndef GFX_PIXMAP
#define GFX_PIXMAP

#include <algorithm>
#include <cmath>
#include <vector>

#include "base/geom/point.h"

#include "color.h"

namespace Gfx
{

class PixMapView
{
public:
	Geom::Size size;
	bool grayscale;
	char *data;

	PixMapView() : grayscale(false), data(nullptr) {}

	size_t bytesPerPixel() const;
	size_t bytesPerLine() const;
	size_t bytesInPixmap() const;
	char *scanLine(size_t y) const;

	Gfx::Color at(int x, int y) const;
	void setAt(int x, int y, const Gfx::Color &color);
};

class PixMap : public PixMapView
{
public:
	typedef std::vector<char> Buffer;

	PixMap(const Geom::Size &size, bool grayscale);
	PixMap(const PixMapView &view);
	PixMap(const PixMapView &view, Buffer buffer);
	PixMap(const PixMap &pixmap);

private:
	Buffer vector;
};

}

#endif
