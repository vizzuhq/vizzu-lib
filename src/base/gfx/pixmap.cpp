#include "pixmap.h"

using namespace Gfx;

size_t PixMapView::bytesPerPixel() const { return grayscale ? 2 : 4; }

size_t PixMapView::bytesPerLine() const
{
	return bytesPerPixel() * static_cast<int>(size.x);
}

size_t PixMapView::bytesInPixmap() const
{
	return bytesPerLine() * static_cast<int>(size.y);
}

char *PixMapView::scanLine(size_t y) const
{
	return data + y * bytesPerLine();
}

Gfx::Color PixMapView::at(int x, int y) const
{
	auto *pix = scanLine(y) + x * bytesPerPixel();
	if (grayscale) {
		uint8_t gray = static_cast<uint8_t>(pix[0]);
		uint8_t alpha = static_cast<uint8_t>(pix[1]);
		return Gfx::Color::RGBA(gray, gray, gray, alpha);
	}
	else {
		uint8_t r = static_cast<uint8_t>(pix[0]);
		uint8_t g = static_cast<uint8_t>(pix[1]);
		uint8_t b = static_cast<uint8_t>(pix[2]);
		uint8_t a = static_cast<uint8_t>(pix[3]);
		return Gfx::Color::RGBA(r, g, b, a);
	}
}

void PixMapView::setAt(int x, int y, const Gfx::Color &color)
{
	auto *pix = scanLine(y) + x * bytesPerPixel();
	if (grayscale) {
		pix[0] = static_cast<char>(static_cast<uint8_t>(color.intensity() * 255));
		pix[1] = static_cast<char>(color.getAlphaByte());
	}
	else {
		pix[0] = static_cast<char>(color.getRedByte());
		pix[1] = static_cast<char>(color.getGreenByte());
		pix[2] = static_cast<char>(color.getBlueByte());
		pix[3] = static_cast<char>(color.getAlphaByte());
	}
}

PixMap::PixMap(const Geom::Size &size, bool grayscale)
{
	this->size = size;
	this->grayscale = grayscale;
	vector.resize(bytesInPixmap());
	data = vector.data();
}

PixMap::PixMap(const PixMapView &view) : PixMapView(view)
{
	vector.assign(data, data + bytesInPixmap());
	data = vector.data();
}

PixMap::PixMap(const PixMapView &view, PixMap::Buffer buffer) :
    PixMapView(view),
    vector(std::move(buffer))
{
	data = vector.data();
}

PixMap::PixMap(const PixMap &pixmap) : PixMapView(pixmap)
{
	vector = pixmap.vector;
	data = vector.data();
}
