#ifndef UTIL_UNIQUEID
#define UTIL_UNIQUEID

#include <cstdint>

namespace Util
{

template<class T>
class UniqueId
{
public:
	UniqueId() {
		static uint64_t nextId = 0;
		id = nextId++;
	}

	explicit UniqueId(uint64_t value) : id(value) {}
	explicit operator uint64_t() const { return id; }

	UniqueId& operator=(uint64_t value)
	{
		id = value;
		return *this;
	}

	bool operator==(const UniqueId &other) const {
		return id == other.id;
	}

	bool operator<(const UniqueId &other) const {
		return id < other.id;
	}

protected:
	uint64_t id;
};

}

#endif

