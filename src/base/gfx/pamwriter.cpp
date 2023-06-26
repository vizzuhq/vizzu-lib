#include "pamwriter.h"

using namespace Gfx;

PamWriter::PamWriter(std::ostream &stream) : stream(stream) {}

void PamWriter::write(const PixMapView &pixmap)
{
	stream << "P7\n"
	       << "WIDTH " << static_cast<int>(pixmap.size.x) << "\n"
	       << "HEIGHT " << static_cast<int>(pixmap.size.y) << "\n"
	       << "DEPTH " << (pixmap.grayscale ? 2 : 4) << "\n"
	       << "MAXVAL 255\n"
	       << "TUPLTYPE "
	       << (pixmap.grayscale ? "GRAYSCALE_ALPHA" : "RGB_ALPHA")
	       << "\n"
	       << "ENDHDR\n";

	stream.write(pixmap.data, pixmap.bytesInPixmap());
}
