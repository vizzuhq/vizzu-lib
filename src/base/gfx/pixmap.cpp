#include "pixmap.h"

using namespace Gfx;

size_t PixMapView::bytesPerPixel() const
{
	return grayscale ? 2 : 4;
}

size_t PixMapView::bytesPerLine() const
{
	return bytesPerPixel() * (int)size.x;
}

size_t PixMapView::bytesInPixmap() const
{
	return bytesPerLine() * (int)size.y;
}

char *PixMapView::scanLine(size_t y) const
{
	return data + y * bytesPerLine();
}

Gfx::Color PixMapView::at(int x, int y) const
{
	auto *pix = scanLine(y) + x * bytesPerPixel();
	if (grayscale) {
		uint8_t gray = *(uint8_t*)pix;
		uint8_t alpha = *(uint8_t*)(pix + 1);
		return Gfx::Color::RGBA(gray, gray, gray, alpha);
	}
	else
	{
		uint8_t r = *(uint8_t*)pix;
		uint8_t g = *(uint8_t*)(pix + 1);
		uint8_t b = *(uint8_t*)(pix + 2);
		uint8_t a = *(uint8_t*)(pix + 3);
		return Gfx::Color::RGBA(r, g, b, a);
	}
}

void PixMapView::setAt(int x, int y, const Gfx::Color &color)
{
	auto *pix = scanLine(y) + x * bytesPerPixel();
	if (grayscale)
	{
		*(uint8_t*)(pix + 0)= color.intensity() * 255;
		*(uint8_t*)(pix + 1) = color.getAlphaByte();
	}
	else
	{
		*(uint8_t*)(pix + 0)= color.getRedByte();
		*(uint8_t*)(pix + 1)= color.getGreenByte();
		*(uint8_t*)(pix + 2)= color.getBlueByte();
		*(uint8_t*)(pix + 3) = color.getAlphaByte();
	}
}

PixMap::PixMap(const Geom::Size &size, bool grayscale)
{
	this->size = size;
	this->grayscale = grayscale;
	vector.resize(bytesInPixmap());
	data = vector.data();
}

PixMap::PixMap(const PixMapView &view)
	: PixMapView(view)
{
	vector.assign(data, data + bytesInPixmap());
	data = vector.data();
}

PixMap::PixMap(const PixMapView &view, PixMap::Buffer buffer)
	: PixMapView(view),
	  vector(std::move(buffer))
{
	data = vector.data();
}

PixMap::PixMap(const PixMap &pixmap) : PixMapView(pixmap)
{
	vector = pixmap.vector;
	data = vector.data();
}
