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
		using FromString = void (*)(Root &, const std::string &);
		using ToString = std::string (*)(const Root &);
		template <class T,
		    std::enable_if_t<Type::isoptional<std::remove_cvref_t<
		        std::invoke_result_t<T &&, Root &>>>::value> * =
		        nullptr>
		consteval Accessor(T &&) :
		    toString(
		        [](const Root &r) mutable
		        {
			        return Conv::toString(
			            std::remove_reference_t<T>{}(r));
		        }),
		    fromString(
		        [](Root &r, const std::string &str) mutable
		        {
			        auto &e = std::remove_reference_t<T>{}(r);
			        e = Conv::parse<std::remove_cvref_t<decltype(e)>>(
			            str);
		        })
		{}

		ToString toString;
		FromString fromString;
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
			return std::ranges::subrange(accessors.begin(),
			    accessors.end());
		}
		else {
			return std::ranges::subrange(
			    accessors.lower_bound(path + "."),
			    accessors.lower_bound(path + "/"));
		}
	}

private:
	ParamRegistry() { Refl::visit<Root>([this] (Accessor&& accessor,
		    std::initializer_list<std::string_view> thePath = {}) {
		    
			std::string currentPath;
			for (auto sv : thePath) {
				if (!currentPath.empty()) currentPath += '.';
				currentPath += sv;
			}

			accessors.try_emplace(std::move(currentPath),
			    std::move(accessor));
		    }); }

	std::map<std::string, Accessor> accessors;
};

}

#endif
