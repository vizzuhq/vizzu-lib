#ifndef LIB_OBJECTREGISTRY_H
#define LIB_OBJECTREGISTRY_H

#include <map>
#include <memory>
#include <shared_mutex>
#include <thread>
#include <variant>

namespace Vizzu
{

using ObjectRegistryHandle = const void *;

template <class... Ts> class ObjectRegistry
{
public:
	using Handle = ObjectRegistryHandle;

	template <class T> Handle reg(std::shared_ptr<T> &&ptr)
	{
		Handle res{std::to_address(ptr)};
		{
			auto lock = std::lock_guard{mutex};
			objects.try_emplace(res, std::move(ptr));
		}
		return res;
	}

	template <class T> std::shared_ptr<T> get(Handle handle)
	{
		{
			auto lock = std::shared_lock{mutex};

			if (auto it = objects.find(handle); it != objects.end())
				if (const auto *casted =
				        std::get_if<std::shared_ptr<T>>(
				            std::addressof(it->second)))
					return *casted;
		}
		throw std::logic_error("No such object exists");
	}

	void unreg(Handle handle)
	{
		if (auto lock = std::lock_guard{mutex}; objects.erase(handle))
			return;

		throw std::logic_error("No such object exists");
	}

private:
	std::map<Handle, std::variant<std::shared_ptr<Ts>...>> objects;
	std::shared_mutex mutex;
};

}

#endif
