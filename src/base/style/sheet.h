#ifndef STYLE_STYLE_H
#define STYLE_STYLE_H

#include <stdexcept>
#include <string>
#include <vector>

#include "base/refl/auto_accessor.h"

namespace Style
{

template <class Params> class Sheet
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

	[[nodiscard]] static auto find(const std::string_view &path)
	{
		return Refl::Access::getAccessor<Params, true>(path);
	}

	[[nodiscard]] static auto prefix_range(const std::string &path)
	{
		const auto &accessors =
		    Refl::Access::getAccessors<Params, true>();

		std::ranges::subrange<typename std::remove_cvref_t<
		    decltype(accessors)>::const_iterator>
		    range;
		if (path.empty())
			range = std::ranges::subrange(accessors.begin(),
			    accessors.end());
		else {
			auto [beg, end] =
			    accessors.equal_range(Prefix{path + "."});
			range = std::ranges::subrange(beg, end);
		}
		return range;
	}

public:
	explicit Sheet(Params defaultParams, Params &activeParams) :
	    defaultParams(std::move(defaultParams)),
	    activeParams(activeParams)
	{}

	[[nodiscard]] const Params &getDefaultParams() const
	{
		return defaultParams;
	}

	[[nodiscard]] Params getFullParams() const;

	static auto listParams()
	{
		return Refl::Access::getAccessorNames<Params, true>();
	}

	void setParams(const std::string &path, const std::string &value)
	{
		setParams(activeParams.get(), path, value);
	}

	static void setParams(Params &params,
	    const std::string &path,
	    const std::string &value)
	{
		if (auto param = find(path).set)
			param(params, value);
		else if (value == "null") {
			auto range = prefix_range(path);

			if (std::empty(range))
				throw std::logic_error(
				    path + ".*" + ": non-existent style parameter"
				    + "(s)");

			for (auto &e : range) e.second.set(params, value);
		}
		else
			throw std::logic_error(
			    path + "/" + value
			    + ": non-existent style parameter");
	}

	[[nodiscard]] static std::string getParam(const Params &params,
	    const std::string &path)
	{
		if (auto param = find(path).get) return param(params);

		throw std::logic_error(
		    path + ": non-existent style parameter");
	}

protected:
	Params defaultParams;
	std::reference_wrapper<Params> activeParams;
};

}

#endif
