#ifndef STYLE_PARAMREGISTRY
#define STYLE_PARAMREGISTRY

#include <functional>
#include <list>
#include <map>
#include <string>
#include <type_traits>

#include "base/conv/parse.h"
#include "base/conv/tostring.h"
#include "base/refl/struct.h"
#include "base/text/smartstring.h"

namespace Style
{

template <typename Root> class ParamRegistry
{
public:
	struct IAccessor
	{
		virtual ~IAccessor(){};
		virtual std::string toString(Root &) = 0;
		virtual void fromString(Root &, const std::string &) = 0;
		size_t offset;
	};

	static ParamRegistry &instance()
	{
		static ParamRegistry registry;
		return registry;
	}

	void visit(const auto &visitor)
	{
		for (auto accessor : accessors) visitor(accessor);
	}

	size_t visit(const auto &visitor, const std::string &pathBegin)
	{
		auto count = 0u;
		for (auto accessor : accessors)
			if (Text::SmartString::startsWith(accessor.first,
			        pathBegin)) {
				visitor(*accessor.second);
				count++;
			}
		return count;
	}

	bool hasParam(const std::string &path) const
	{
		return accessors.find(path) != accessors.end();
	}

	std::list<std::string> listParams() const
	{
		std::list<std::string> list;
		for (const auto &accessor : accessors)
			list.push_back(accessor.first);
		return list;
	}

	void visit(const std::string &path, const auto &visitor)
	{
		visitor(*accessors.at(path));
	}

private:
	template <typename T> struct Accessor : IAccessor
	{
		std::string toString(Root &root) override
		{
			auto &value = *reinterpret_cast<T *>(
			    reinterpret_cast<std::byte *>(&root) + this->offset);

			return value ? Conv::toString(*value) : "null";
		};

		void fromString(Root &root, const std::string &s) override
		{
			auto &value = *reinterpret_cast<T *>(
			    reinterpret_cast<std::byte *>(&root) + this->offset);

			if (s == "null")
				value.reset();
			else
				value = Conv::parse<typename T::value_type>(s);
		}
	};

	struct Proxy
	{
		Proxy(ParamRegistry &registry,
		    std::byte *base,
		    const std::string &path = std::string()) :
		    registry(registry),
		    base(base),
		    currentPath(path)
		{}

		template <typename T>
		Proxy &operator()(T &value, const std::string &name)
		{
			auto path =
			    currentPath + (currentPath.empty() ? "" : ".") + name;

			if constexpr (Refl::isReflectable<T, Proxy>) {
				auto proxy = Proxy(registry, base, path);
				value.visit(proxy);
			}
			else {
				std::byte *ptr =
				    reinterpret_cast<std::byte *>(&value);
				IAccessor *accessor = new Accessor<T>();
				accessor->offset = ptr - base;
				registry.accessors.emplace(path, accessor);
			}
			return *this;
		}

		ParamRegistry &registry;
		std::byte *base;
		std::string currentPath;
	};

	ParamRegistry()
	{
		/* Note: any typesafe solution resulted in at least 50kbyte
		   wasm binary size for 100 param. This implementation takes
		   up to 20kbyte. */
		Root root;
		auto proxy =
		    Proxy(*this, reinterpret_cast<std::byte *>(&root));
		root.visit(proxy);
	}

	std::map<std::string, IAccessor *> accessors;
};

}

#endif
