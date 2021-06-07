#ifndef UTIL_OBJECTCACHE
#define UTIL_OBJECTCACHE

#include <cstdint>
#include <unordered_map>
#include <optional>

namespace Util
{

template <typename T> struct Cached { T value; };

template <typename T>
Cached<T> makeCached(const T& value) { return Cached<T>{ value }; }

template <typename T>
class ObjectCache
{
public:
	typedef uint32_t Id;

	typedef std::pair<Id, std::optional<T>>Handle;

	Handle get(const T& value)
	{
		Handle res;
		auto it = ids.find(value);
		bool exists = it != ids.end();
		if (!exists) {
			res.first = ids.size();
			res.second = value;
			ids.insert({ value, res.first });
			values.insert({ res.first, value });
		} else {
			res.first = it->second;
		}
		return res;
	}

	T get(const Handle &handle)
	{
		if (handle.second) {
			ids.insert({ *handle.second, handle.first });
			values.insert({ handle.first, *handle.second });
			return *handle.second;
		} else {
			return values.at(handle.first);
		}
	}

private:
	std::unordered_map<T, Id> ids;
	std::unordered_map<Id, T> values;

};

}

#endif
