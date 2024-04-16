#ifndef STYLE_STYLE_H
#define STYLE_STYLE_H

#include <stdexcept>
#include <string>
#include <vector>

#include "base/style/paramregistry.h"

namespace Style
{

template <class Params> class Sheet
{
public:
	explicit Sheet(Params defaultParams) :
	    defaultParams(std::move(defaultParams)),
	    activeParams(nullptr)
	{}

	void setActiveParams(Params &params) { activeParams = &params; }

	[[nodiscard]] const Params &getDefaultParams() const
	{
		return defaultParams;
	}

	[[nodiscard]] Params getFullParams() const;

	static std::list<std::string> paramList()
	{
		return Style::ParamRegistry<Params>::instance().listParams();
	}

	void setParams(const std::string &path, const std::string &value)
	{
		if (!activeParams)
			throw std::logic_error("no active parameters set");

		setParams(*activeParams, path, value);
	}

	static void setParams(Params &params,
	    const std::string &path,
	    const std::string &value)
	{
		auto &registry = Style::ParamRegistry<Params>::instance();
		if (auto param = registry.find(path)) {
			param->fromString(params, value);
		}
		else if (value == "null") {
			auto range = registry.prefix_range(path);

			if (std::empty(range))
				throw std::logic_error(
				    path + ".*" + ": non-existent style parameter"
				    + "(s)");

			for (auto &e : range) e.second.fromString(params, value);
		}
		else
			throw std::logic_error(
			    path + "/" + value
			    + ": non-existent style parameter");
	}

	[[nodiscard]] static std::string getParam(const Params &params,
	    const std::string &path)
	{
		auto &paramReg = Style::ParamRegistry<Params>::instance();

		if (auto param = paramReg.find(path))
			return param->toString(params);

		throw std::logic_error(
		    path + ": non-existent style parameter");
	}

protected:
	Params defaultParams;
	Params *activeParams;
};

}

#endif
