#ifndef IO_FILENAME
#define IO_FILENAME

#include <string>
#include <algorithm>

namespace IO
{

class FileName
{
public:
	static std::string extension(const std::string &filename)
	{
		auto idx = filename.rfind('.');
		return idx != std::string::npos ? filename.substr(idx+1) : std::string();
	}

	static std::string basename(const std::string &filename)
	{
		auto dotPos = filename.rfind('.');
		auto slashPos = filename.rfind('/');
		auto backslashPos = filename.rfind('\\');

		auto end = dotPos != std::string::npos ? dotPos : filename.size();
		auto slashBegin = slashPos != std::string::npos ? slashPos+1 : 0;
		auto bslashBegin = backslashPos != std::string::npos ? backslashPos+1 : 0;

		if (slashBegin > end) end = filename.size();
		if (bslashBegin > end) end = filename.size();

		auto begin = std::max(slashBegin, bslashBegin);

		return filename.substr(begin, end - begin);
	}

};

}

#endif
