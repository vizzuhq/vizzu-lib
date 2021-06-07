
#ifndef IO_TAR
#define IO_TAR

#include <string>
#include <map>
#include <vector>
#include <istream>

#include "ifilesystem.h"
#include "rangestream.h"

namespace IO
{
namespace Tar
{

struct Header;

class Reader : public IFileSystem
{
public:
	Reader(std::istream &input);
	std::string getContent(const std::string &fileName) override;
	std::shared_ptr<std::istream> getStream(const std::string &fileName) override;
	[[noreturn]] std::vector<std::string> listFiles(const std::string &globPattern) override;
	[[noreturn]] std::vector<std::string> listDirs(const std::string &globPattern) override;
	uint64_t getFileSize(const std::string &filename) override;
	bool fileExists(const std::string &filename) override;
	[[noreturn]] bool dirExists(const std::string &filename) override;

private:
	std::istream &stream;
	std::map<std::string, std::istream::pos_type> files;

	std::istream::pos_type getFilePos(const std::string &fileName);
	void readHeader(const std::string &fileName, Header &header);
};

}
}

#endif
