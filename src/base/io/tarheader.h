#ifndef IO_TARHEADER
#define IO_TARHEADER

#include <string>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <chrono>

namespace IO
{
namespace Tar
{

#pragma pack(push, 1)

template <size_t size>
struct Integer
{
	explicit operator uint64_t() const {
		try {
			std::string octal((char*)this, size);
			return std::stoll(octal, nullptr, 8);
		}
		catch (...)
		{
			throw std::logic_error(std::string("invalid tar format integer: ")
								   + std::string((const char*)this, size));
		}
	}

	Integer<size> &operator=(uint64_t value) {
		snprintf((char*)this, size, 
				("%0" + std::to_string(size-1) +"o").c_str(),
				(unsigned)value);
		return *this;
	}
private:
	char data[size];
};

template <size_t size>
struct String
{
	explicit operator std::string() const {
		return data[size-1] == 0
			? std::string(&(data[0]))
			: std::string(&(data[0]), 100);
	}

	String<size> &operator=(const std::string &value) {
		snprintf(data, size, "%s", value.c_str());
		return *this;
	}

	bool empty() const { return data[0] == 0; }

private:
	char data[size];
};

struct Header
{
	String<100> fileName;
	Integer<8> fileMode;
	Integer<8> ownerId;
	Integer<8> groupId;
	Integer<12> fileSize;
	Integer<12> lastModificationTime;
	Integer<8> checkSum;
	enum : uint8_t { none = '0', hard = '1', symbolic = '2' } linkIndicator;
	String<100> linkName;
	char padding[255];

	Header()
	{
		using namespace std::chrono;
		::memset(this, 0, sizeof(Header));
		fileMode = 0777777ull;
		ownerId = 0ull;
		groupId = 0ull;
		lastModificationTime = 
			duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
		linkIndicator = none;
	}

	bool empty() const { return fileName.empty(); }

	char *data() const {
		return (char*)this + sizeof(Header);
	}

	void setChecksum() {
		::memset((char*)&checkSum, ' ', sizeof(checkSum));
		auto sum = 0ull;
		for (auto *pos = (uint8_t *)this; pos < (uint8_t *)(this + 1); pos++)
			sum += *pos;
		checkSum = sum;
	}

	uint64_t paddedFileSize() const {
		try {
			int chunks = (int)std::ceil((double)(uint64_t)fileSize / sizeof(Header));
			return chunks * sizeof(Header);
		}
		catch(...) {
			throw std::logic_error(
					std::string("invalid file size in tar file: ")
					+ std::string(fileName));
		}
	}

	Header *nextHeader() const {
		return (Header *)(data() + paddedFileSize());
	}

};

#pragma pack(pop)

static_assert(sizeof(Header) == 512, "Tar header size mismatch");

}
}

#endif
