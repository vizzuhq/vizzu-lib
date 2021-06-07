#include "base/io/tarreader.h"

#include <cassert>
#include <cmath>
#include <set>
#include <stdexcept>

#include "tarheader.h"

namespace IO::Tar
{

std::vector<std::string> Reader::listFiles(const std::string &) {
	throw std::logic_error("Not supported");
}

std::vector<std::string> Reader::listDirs(const std::string &)
{
	throw std::logic_error("Not supported");
}

uint64_t Reader::getFileSize(const std::string &filename) {
	Header header;
	readHeader(filename, header);
	return (uint64_t)header.fileSize;
}

bool Reader::fileExists(const std::string &filename) {
	return files.find(filename) != files.end();
}

bool Reader::dirExists(const std::string &)
{
	throw std::logic_error("Not supported");
}

Reader::Reader(std::istream &input)
	: stream(input)
{
	stream.seekg(0, std::ios::beg);
	Header header;
	while (!stream.eof()) {
		auto pos = stream.tellg();
		stream.read((char*)&header, sizeof(Header));
		if (stream.gcount() < (int64_t)sizeof(Header)) return;
		if (stream.bad()) throw std::logic_error("Tar reading error");
		if (stream.eof()) return;
		if (!header.empty()) {
			files[(std::string)header.fileName] = pos;
			stream.seekg(header.paddedFileSize(), std::ios::cur);
		}
		else {
			stream.seekg(sizeof(Header), std::ios::cur);
		}
	}
}

std::istream::pos_type Reader::getFilePos(const std::string &fileName) {
	try {
		return files.at(fileName);
	} catch (std::out_of_range &) {
		throw std::out_of_range("File can not found in tar file: " + fileName);
	}
}

std::string Reader::getContent(const std::string &fileName) {
	Header header;
	readHeader(fileName, header);
	std::string res;
	res.resize((uint64_t)header.fileSize);
	stream.read((char*)res.data(), (uint64_t)header.fileSize);
	return res;
}

std::shared_ptr<std::istream>
Reader::getStream(const std::string &fileName)
{
	Header header;
	readHeader(fileName, header);
	return std::make_shared<RangeIStream>
		(stream, stream.tellg(), (uint64_t)header.fileSize);
}

void Reader::readHeader(const std::string &fileName, Header &header)
{
	auto pos = getFilePos(fileName);
	stream.clear();
	stream.seekg(pos);
	stream.read((char*)&header, sizeof(Header));
}

}
