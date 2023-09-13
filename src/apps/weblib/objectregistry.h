#ifndef LIB_OBJECTREGISTRY_H
#define LIB_OBJECTREGISTRY_H

#include <any>
#include <memory>
#include <unordered_map>

namespace Vizzu
{

class ObjectRegistry
{
public:
	using Handle = const void *;

	template <class T> Handle reg(std::shared_ptr<T> ptr)
	{
		Handle res{ptr.get()};
		objects.emplace(res, std::move(ptr));
		return res;
	}

	template <class T> const std::shared_ptr<T> &get(Handle handle)
	{
		if (auto it = objects.find(handle); it != objects.end())
			return std::any_cast<const std::shared_ptr<T> &>(
			    it->second);

		throw std::logic_error("No such object exists");
	}

	void unreg(Handle handle)
	{
		if (!objects.erase(handle))
			throw std::logic_error("No such object exists");
	}

private:
	std::unordered_map<Handle, std::any> objects;
};

}

#endif
