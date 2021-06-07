#include "svgregistry.h"

#include <fstream>

#include "base/io/filename.h"

using namespace Gfx;

SvgRegistry SvgRegistry::singleton;

const Svg &SvgRegistry::preload(const std::string &filename)
{
	std::ifstream file(filename, std::ios::binary);

	return preload(file, filename);
}

const Svg &SvgRegistry::preload(std::istream &file, const std::string &filename)
{
	std::string data(std::istreambuf_iterator<char>{file}, {});
	auto it = svgs.insert({ filename, data }).first;
	return it->second;
}

const Svg &SvgRegistry::preload(std::string &content, const std::string &filename)
{
	auto it = svgs.insert({ filename, content }).first;
	return it->second;
}

const Svg &SvgRegistry::get(const std::string &filename)
{
	auto it = svgs.find(filename);
	if (it != svgs.end())
		return it->second;

	return preload(filename);
}

void SvgRegistry::purge(const std::string &filename)
{
	auto it = svgs.find(filename);
	if (it != svgs.end())
		svgs.erase(it);
}

void SvgRegistry::saveAll(IO::OFileSystem &fileSystem) const
{
	for (const auto &pair: svgs)
	{
		const auto &fullPathName = pair.first;
		const auto &svg = pair.second;

		auto file = fileSystem.newFile(fullPathName);
		file->stream().write(svg.data.data(), svg.data.size());
	}
}

SvgRegistry::Citerator SvgRegistry::begin() const
{
	return svgs.begin();
}

SvgRegistry::Citerator SvgRegistry::end() const
{
	return svgs.end();
}
