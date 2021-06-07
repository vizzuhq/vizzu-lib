#ifndef IO_OFILESYSTEM
#define IO_OFILESYSTEM

#include <string>
#include <memory>
#include <ostream>

namespace IO
{

struct OFileSystem
{
	struct File
	{
		virtual ~File() {}
		virtual std::ostream &stream() = 0;
	};

	virtual ~OFileSystem() {}
	virtual std::shared_ptr<File> newFile(const std::string &filename) = 0;
	virtual void createDir(const std::string &path) = 0;
	virtual void deleteDir(const std::string &path) = 0;
	virtual void copy(const std::string &path1, const std::string &path2) = 0;
	virtual void move(const std::string &path1, const std::string &path2) = 0;
};

}

#endif 
