#ifndef STYLE_PARAMREGISTRY
#define STYLE_PARAMREGISTRY

#include <functional>
#include <map>
#include <string>
#include <type_traits>
#include <utility>

#include "base/conv/parse.h"
#include "base/conv/tostring.h"
#include "base/text/smartstring.h"

namespace Style
{

template <typename Root> class ParamRegistry
{
	struct Prefix
	{
		std::string prefix;

		friend bool operator<(const std::string &lhs,
		    const Prefix &rhs)
		{
			return std::string_view{lhs}.substr(0, rhs.prefix.size())
			     < std::string_view{rhs.prefix};
		}

		friend bool operator<(const Prefix &lhs,
		    const std::string &rhs)
		{
			return std::string_view{lhs.prefix}
			     < std::string_view{rhs}.substr(0, lhs.prefix.size());
		}
	};

public:
	struct Accessor
	{
		using FromString = void(Root &, const std::string &);
		using ToString = std::string(const Root &);
		template <class T>
		    requires(Type::is_optional_v<std::remove_cvref_t<
		                    std::invoke_result_t<T &&, Root &>>>)
		constexpr inline
		    __attribute__((always_inline)) explicit Accessor(T &&t) :
		    toString(
		        [t](const Root &r)
		        {
			        return Conv::toString(t(r));
		        }),
		    fromString(
		        [t](Root &r, const std::string &str)
		        {
			        auto &e = t(r);
			        e = Conv::parse<std::remove_cvref_t<decltype(e)>>(
			            str);
		        })
		{}

		Accessor(Accessor &&) = delete;
		Accessor &operator=(Accessor &&) = delete;
		Accessor(const Accessor &) = delete;
		Accessor &operator=(const Accessor &) = delete;

		std::function<ToString> toString;
		std::function<FromString> fromString;
	};

	static const ParamRegistry &instance()
	{
		static const ParamRegistry registry;
		return registry;
	}

	[[nodiscard]] auto listParams() const
	{
		return std::ranges::views::keys(accessors);
	}

	[[nodiscard]] const Accessor *find(const std::string &path) const
	{
		if (auto it = accessors.find(path); it != std::end(accessors))
		    [[likely]]
			return std::addressof(it->second);
		return nullptr;
	}

	[[nodiscard]] auto prefix_range(const std::string &path) const
	{
		if (path.empty()) {
			return std::ranges::subrange(accessors.begin(),
			    accessors.end());
		}
		auto [b, e] = accessors.equal_range(Prefix{path + "."});
		return std::ranges::subrange(b, e);
	}

private:
	ParamRegistry(); // NOLINT

	std::map<std::string, Accessor, std::less<>> accessors;
};

template <class Root, class U>
concept IsAccessor =
    std::is_constructible_v<typename ParamRegistry<Root>::Accessor,
        U>;

}

#endif
