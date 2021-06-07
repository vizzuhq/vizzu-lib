
#ifndef IO_IFILESYSTEM
#define IO_IFILESYSTEM

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <ostream>

namespace IO
{

class IFileSystem {
public:
	virtual ~IFileSystem() {}
	virtual std::string getContent(const std::string &filename) = 0;
	virtual std::shared_ptr<std::istream> getStream(const std::string &fileName) = 0;
	virtual std::vector<std::string> listFiles(const std::string &globPattern) = 0;
	virtual std::vector<std::string> listDirs(const std::string &globPattern) = 0;
	virtual uint64_t getFileSize(const std::string &filename) = 0;
	virtual bool fileExists(const std::string &filename) = 0;
	virtual bool dirExists(const std::string &filename) = 0;
};

}

#endif
