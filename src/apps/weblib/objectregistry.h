#ifndef LIB_OBJECTREGISTRY_H
#define LIB_OBJECTREGISTRY_H

#include <memory>
#include <unordered_map>

namespace Vizzu
{

class ObjectRegistry
{
public:
	typedef void *Handle;

	Handle reg(std::shared_ptr<void> ptr)
	{
		Handle handle = ptr.get();
		objects.emplace(handle, ptr);
		return handle;
	}

	template <class T> std::shared_ptr<T> get(Handle handle)
	{
		auto it = objects.find(handle);
		if (it == objects.end() || !it->second)
			throw std::logic_error("No such object exists");
		return std::static_pointer_cast<T>(it->second);
	}

	void unreg(Handle handle)
	{
		auto it = objects.find(handle);
		if (it == objects.end())
			throw std::logic_error("No such object exists");
		objects.erase(it);
	}

private:
	std::unordered_map<void *, std::shared_ptr<void>> objects;
};

}

#endif
