#include "filesystem.h"

#include <cassert>
#include <cerrno>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>

#if(defined(WIN32) || defined(__MINGW32__))
#include <direct.h>
#include <windows.h>
#else
#include <dirent.h>
#include <glob.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

namespace IO
{

std::string FileSystem::getContent(const std::string &filename) {
	std::ifstream in(filename);
	std::stringstream buffer;
	buffer << in.rdbuf();
	return buffer.str();
}

std::shared_ptr<std::istream> FileSystem::getStream(const std::string &fileName) {
	return std::make_shared<std::ifstream>(fileName, std::ios::binary);
}

#if(defined(WIN32) || defined(__MINGW32__))

std::vector<std::string> FileSystem::listFiles(const std::string &globPattern)
{
	std::vector<std::string> files;
	WIN32_FIND_DATAA fd;
	HANDLE hFind = FindFirstFileA(globPattern.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				files.push_back(fd.cFileName);
			}
		} while (FindNextFileA(hFind, &fd));
		FindClose(hFind);
	}
	return files;
}

std::vector<std::string> FileSystem::listDirs(const std::string &globPattern)
{
	std::vector<std::string> dirs;
	WIN32_FIND_DATAA fd;
	HANDLE hFind = FindFirstFileA(globPattern.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY &&
				strcmp(fd.cFileName, ".") != 0 &&
				strcmp(fd.cFileName, "..") != 0)
			{
				dirs.push_back(fd.cFileName);
			}
		} while (FindNextFileA(hFind, &fd));
		FindClose(hFind);
	}
	return dirs;
}

bool FileSystem::fileExists(const std::string &filename)
{
	WIN32_FIND_DATAA FindFileData;
	HANDLE handle = FindFirstFileA(filename.c_str(), &FindFileData) ;
	bool found = handle != INVALID_HANDLE_VALUE && (FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) == 0;
	if(found) FindClose(handle);
	return found;
}

bool FileSystem::dirExists(const std::string &dirName)
{
	WIN32_FIND_DATAA FindFileData;
	HANDLE handle = FindFirstFileA(dirName.c_str(), &FindFileData);
	bool found = handle != INVALID_HANDLE_VALUE && (FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) != 0;
	if(found) FindClose(handle);
	return found;
}

#else

std::vector<std::string> FileSystem::listFiles(const std::string &globPattern)
{
	std::vector<std::string> files;
	glob_t glob_result;
	glob(globPattern.c_str(), 0, nullptr, &glob_result);
    for(size_t i = 0; i < glob_result.gl_pathc; i++) {
		files.emplace_back(glob_result.gl_pathv[i]);
	}
	globfree(&glob_result);
	return files;
}

std::vector<std::string> FileSystem::listDirs(const std::string &)
{
	throw std::logic_error("listDirs is not implemented");
}

bool FileSystem::fileExists(const std::string &filename)
{
	struct stat buffer;
	return (stat(filename.c_str(), &buffer) == 0);
}

bool FileSystem::dirExists(const std::string &dirName)
{
	DIR *dir = opendir(dirName.c_str());
	if (dir) {
		closedir(dir);
		return true;
	} else return false;
}

#endif

uint64_t FileSystem::getFileSize(const std::string &filename) {
	std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
	return in.tellg();
}

}
