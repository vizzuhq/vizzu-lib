
#ifndef IO_TARBUILDER
#define IO_TARBUILDER

#include "ofilesystem.h"

namespace IO
{
namespace Tar
{

struct Header;

class Builder : public IO::OFileSystem
{
	class TarFile : public File {
		friend class Builder;
	public:
		std::ostream &stream() override;
	private:
		std::string name;
		bool closed;
		std::ostream &outStream;
		std::ostream::pos_type headerPos;

		TarFile(std::string filename, std::ostream &stream);
		void close();
	};

public:
	Builder(std::ostream &out);
	~Builder() override;

	std::shared_ptr<File> newFile(const std::string &filename) override;
	void createDir(const std::string &) override {}
	void deleteDir(const std::string &) override { throw std:: logic_error("not supported operation"); }
	void copy(const std::string &, const std::string &) override { throw std:: logic_error("not supported operation"); }
	void move(const std::string &, const std::string &) override { throw std:: logic_error("not supported operation"); }
	virtual void close();

protected:
	std::ostream &stream;

private:
	std::shared_ptr<TarFile> actFile;
};

}
}

#endif
