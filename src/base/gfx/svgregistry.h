#ifndef GFX_SVGREGISTRY
#define GFX_SVGREGISTRY

#include <string>
#include <iostream>
#include <map>
#include <functional>
#include "base/io/ofilesystem.h"
#include "svg.h"

namespace Gfx
{

class SvgRegistry
{
public:

	typedef std::map<std::string, Svg> Svgs;
	typedef Svgs::const_iterator Citerator;

	static SvgRegistry singleton;

	const Svg &preload(const std::string &filename);
	const Svg &preload(std::istream &file, const std::string &filename);
	const Svg &preload(std::string &content, const std::string &filename);

	const Svg &get(const std::string &filename);
	void purge(const std::string &filename);

	void saveAll(IO::OFileSystem &fileSystem) const;

	Citerator begin() const;
	Citerator end() const;

private:
	Svgs svgs;
};

}

#endif
