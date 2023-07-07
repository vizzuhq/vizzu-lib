#ifndef STYLE_PARAMREGISTRY
#define STYLE_PARAMREGISTRY

#include <functional>
#include <list>
#include <map>
#include <string>
#include <type_traits>

#include "base/conv/parse.h"
#include "base/conv/tostring.h"
#include "base/refl/auto_struct.h"
#include "base/text/smartstring.h"

namespace Style
{

template <typename Root> class ParamRegistry
{
public:
	struct Accessor
	{
		template <class T>
		explicit Accessor(T &&t) :
		    toString(
		        [t](const Root &r) mutable
		        {
			        return Conv::toString(t(r));
		        }),
		    fromString(
		        [t](Root &r, const std::string &str) mutable
		        {
			        auto &e = t(r);
			        e = Conv::parse<std::remove_cvref_t<decltype(e)>>(
			            str);
		        })
		{}

		Accessor(const Accessor &) = delete;
		Accessor &operator=(const Accessor &) = delete;

		std::function<std::string(const Root &)> toString;
		std::function<void(Root &, const std::string &)> fromString;
	};

	static ParamRegistry &instance()
	{
		static ParamRegistry registry;
		return registry;
	}

	std::list<std::string> listParams() const
	{
		std::list<std::string> list;
		for (const auto &accessor : accessors)
			list.push_back(accessor.first);
		return list;
	}

	Accessor *find(const std::string &path)
	{
		if (auto it = accessors.find(path); it != std::end(accessors))
		    [[likely]]
			return std::addressof(it->second);
		return nullptr;
	}

	auto prefix_range(const std::string &path)
	{
		if (path.empty()) {
			return std::pair{accessors.begin(), accessors.end()};
		}
		else {
			return std::pair{accessors.lower_bound(path + "."),
			    accessors.lower_bound(path + "/")};
		}
	}

private:
	struct Proxy
	{
		Proxy(ParamRegistry &registry) : registry(registry) {}

		template <typename G>
		auto operator()(G &&getter,
		    std::initializer_list<std::string_view> thePath = {})
            -> std::enable_if_t<Type::isoptional<std::remove_cvref_t<std::invoke_result_t<G, Root&>>>::value>
		{
			std::string currentPath;
			for (auto sv : thePath) {
				if (!currentPath.empty()) currentPath += '.';
				currentPath += sv;
			}

			registry.accessors.try_emplace(std::move(currentPath),
			    getter);
		}

		ParamRegistry &registry;
	};

	ParamRegistry() { Refl::visit<Root>(Proxy(*this)); }

	std::map<std::string, Accessor> accessors;
};

}

#endif
