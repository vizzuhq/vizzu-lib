#include "pixmapregistry.h"

#include <fstream>

#include "base/io/filename.h"
#include "pamreader.h"
#include "pamwriter.h"

using namespace Gfx;

PixMapRegistry PixMapRegistry::singleton;

PixMapView PixMapRegistry::preload(const std::string &filename, ImageFormat format)
{
	std::ifstream file(filename, std::ios::binary);

	return preload(file, filename, format);
}

PixMapView PixMapRegistry::preload(std::istream &file, const std::string &filename, ImageFormat format)
{
	if (format != ImageFormat::pam) throw std::logic_error("not supported format");

	std::vector<char> data(std::istreambuf_iterator<char>{file}, {});
	auto view = PamReader::parse(data.data(), data.size());

	auto it = pixmaps.insert({ filename, PixMap(view)}).first;

	return it->second;
}

PixMapView PixMapRegistry::preload(std::string &content, const std::string &filename, ImageFormat format)
{
	if (format != ImageFormat::pam) throw std::logic_error("not supported format");

	auto view = PamReader::parse(content.data(), content.size());

	auto it = pixmaps.insert({ filename, PixMap(view)}).first;

	return it->second;
}

PixMapView PixMapRegistry::get(const std::string &filename)
{
	static const std::map<std::string, ImageFormat> formats{
		{ "pam", ImageFormat::pam }
	};

	auto it = pixmaps.find(filename);
	if (it != pixmaps.end())
		return it->second;

	// todo: move this to preload
	auto ext = IO::FileName::extension(filename);
	auto f = formats.find(ext);
	if (f == formats.end())
	{
		if (reader) {
			auto pixmap = reader(filename);
			auto it = pixmaps.insert({ filename, pixmap }).first;
			return it->second;
		}
		else throw std::logic_error("not supported image format: " + filename);
	}
	else
	{
		auto format = f->second;
		return preload(filename, format);
	}
}

void PixMapRegistry::purge(const std::string &filename)
{
	auto it = pixmaps.find(filename);
	if (it != pixmaps.end())
		pixmaps.erase(it);
}

void PixMapRegistry::saveAll(IO::OFileSystem &fileSystem) const
{
	for (const auto &pair: pixmaps)
	{
		const auto &fullPathName = pair.first;
		const auto &pixmap = pair.second;

		auto file = fileSystem.newFile(pamName(fullPathName));
		PamWriter pamFile(file->stream());
		pamFile.write(pixmap);
	}
}

PixMapRegistry::Citerator PixMapRegistry::begin() const
{
	return pixmaps.begin();
}

PixMapRegistry::Citerator PixMapRegistry::end() const
{
	return pixmaps.end();
}

std::string PixMapRegistry::pamName(const std::string &fullPathName)
{
	return IO::FileName::basename(fullPathName) + ".pam";
}
