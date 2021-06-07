#include "pamreader.h"

#include <streambuf>

using namespace Gfx;

struct MemBuffer : std::streambuf
{
	MemBuffer(char* begin, char* end) { this->setg(begin, begin, end); }
};

static
std::string getValue(const std::string &line, const std::string &key)
{
	if (line.substr(0, key.size()) == key)
		return line.substr(key.size());
	else
		return std::string();
}

PixMapView PamReader::parse(char *data, size_t size)
{
	MemBuffer buffer(data, data + size);
	std::istream in(&buffer);

	auto res = ParsedHeader(in);
	res.pixmap.data = data + res.headerSize;

	return res.pixmap;
}

PixMap PamReader::parse(std::istream &in)
{
	auto res = ParsedHeader(in);
	PixMap::Buffer buffer;
	buffer.resize(res.pixmap.bytesInPixmap());
	in.read(buffer.data(), res.pixmap.bytesInPixmap());
	return PixMap(res.pixmap, buffer);
}

PamReader::ParsedHeader::ParsedHeader(std::istream &in)
{
	headerSize = 0;

	bool widthSet = false;
	bool heightSet = false;
	bool typeSet = false;
	std::string line;
	while (std::getline(in, line))
	{
		headerSize += line.size() + 1;

		if (line == "ENDHDR") break;

		std::string value;

		if (value = getValue(line, "WIDTH "), !value.empty())
		{
			pixmap.size.x = std::stoi(value);
			widthSet = true;
		}
		else if (value = getValue(line, "HEIGHT "), !value.empty())
		{
			pixmap.size.y = std::stoi(value);
			heightSet = true;
		}
		else if (value = getValue(line, "TUPLTYPE "), !value.empty())
		{
			pixmap.grayscale = value == "GRAYSCALE_ALPHA";
			typeSet = true;
		}
	}
	if (!widthSet || !heightSet || !typeSet)
		throw std::logic_error("incomplete PAM header");
}
