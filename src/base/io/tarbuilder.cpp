#include "tarbuilder.h"

#include <array>

#include "tarheader.h"

namespace IO::Tar {

static std::array<char, sizeof(Header)> emptyChunk{0};

Builder::TarFile::TarFile(std::string filename, std::ostream &stream)
	: name(std::move(filename)), outStream(stream)
{
	headerPos = stream.tellp();
	closed = false;
	stream.write(emptyChunk.data(), emptyChunk.size());
}

void Builder::TarFile::close()
{
	if (closed) return;

	auto actPos = outStream.tellp();
	auto fullSize = actPos - headerPos;
	auto fileSize = fullSize - sizeof(Header);

	outStream.seekp(headerPos);
	Header header;
	header.fileName = name;
	header.fileSize = (uint64_t)fileSize;
	header.setChecksum();
	outStream.write((const char*)&header, sizeof(Header));
	outStream.seekp(actPos);

	auto paddingSize = header.paddedFileSize() - fileSize;
	outStream.write(emptyChunk.data(), paddingSize);

	closed = true;
}

std::ostream &Builder::TarFile::stream()
{
	if (closed) throw std::logic_error("write attempt to closed tar chunk");
	return outStream;
}

Builder::Builder(std::ostream &out)
	: stream(out)
{}

Builder::~Builder()
{
	if (actFile) actFile->close();
}

std::shared_ptr<IO::OFileSystem::File>
Builder::newFile(const std::string &filename)
{
	if (actFile) actFile->close();
	actFile = std::shared_ptr<TarFile>(new TarFile(filename, stream));
	return actFile;
}

void Builder::close()
{
	if (actFile) actFile->close();
	actFile.reset();
}

}
