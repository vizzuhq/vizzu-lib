#ifndef GFX_PIXMAPREGISTRY
#define GFX_PIXMAPREGISTRY

#include <string>
#include <iostream>
#include <map>
#include <functional>
#include "base/io/ofilesystem.h"
#include "base/gfx/pixmap.h"

namespace Gfx
{

enum class ImageFormat : uint32_t { pam };

class PixMapRegistry
{
public:

	typedef std::map<std::string, PixMap> Pixmaps;
	typedef Pixmaps::const_iterator Citerator;

	static PixMapRegistry singleton;

	std::function<PixMap(const std::string &)> reader;

	PixMapView preload(const std::string &filename, ImageFormat format);
	PixMapView preload(std::istream &file, const std::string &filename, ImageFormat format);
	PixMapView preload(std::string &content, const std::string &filename, ImageFormat format);

	PixMapView get(const std::string &filename);
	void purge(const std::string &filename);

	void saveAll(IO::OFileSystem &fileSystem) const;

	Citerator begin() const;
	Citerator end() const;

	static std::string pamName(const std::string &fullPathName);

private:
	Pixmaps pixmaps;
};

}

#endif
