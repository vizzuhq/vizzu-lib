
#ifndef IO_FILESYSTEM
#define IO_FILESYSTEM

#include "ifilesystem.h"

namespace IO
{

class FileSystem : public IFileSystem
{
public:
	std::string getContent(const std::string &filename) override;
	std::shared_ptr<std::istream> getStream(const std::string &fileName) override;
	std::vector<std::string> listFiles(const std::string &globPattern) override;
	[[noreturn]] std::vector<std::string> listDirs(const std::string &globPattern) override;
	uint64_t getFileSize(const std::string &filename) override;
	bool fileExists(const std::string &filename) override;
	bool dirExists(const std::string &dirName) override;
};

}

#endif
