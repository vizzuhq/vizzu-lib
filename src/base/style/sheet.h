#ifndef STYLE_STYLE_H
#define STYLE_STYLE_H

#include <stdexcept>
#include <string>
#include <vector>

#include "base/refl/auto_struct.h"
#include "base/style/parammerger.h"
#include "base/style/paramregistry.h"

namespace Style
{

template <class Params> class Sheet
{
public:
	Sheet(const Params &defaultParams) :
	    defaultParams(defaultParams),
	    activeParams(nullptr)
	{}

	void setActiveParams(Params &params) { activeParams = &params; }

	const Params &getDefaultParams() const { return defaultParams; }

	Params getFullParams() const
	{
		if (!activeParams)
			throw std::logic_error("no active parameters set");

		return ParamMerger::merge(Params{defaultParams},
		    *activeParams);
	}

	static std::list<std::string> paramList()
	{
		return Style::ParamRegistry<Params>::instance().listParams();
	}

	void setParamDefault(const std::string &path,
	    const std::string &value)
	{
		setParam(defaultParams, path, value);
	}

	void setParam(const std::string &path, const std::string &value)
	{
		if (!activeParams)
			throw std::logic_error("no active parameters set");

		setParam(*activeParams, path, value);
	}

	void setParams(const std::string &path, const std::string &value)
	{
		if (!activeParams)
			throw std::logic_error("no active parameters set");

		setParams(*activeParams, path, value);
	}

	static bool hasParam(const std::string &path)
	{
		return Style::ParamRegistry<Params>::instance().hasParam(
		    path);
	}

	static void setParam(Params &params,
	    const std::string &path,
	    const std::string &value)
	{
		if (!hasParam(path))
			throw std::logic_error(
			    path + "/" + value
			    + ": non-existent style parameter");

		Style::ParamRegistry<Params>::instance().visit(path,
		    [&](auto &p)
		    {
			    p.fromString(params, value);
		    });
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
			auto [begin, end] = registry.prefix_range(path);

			if (begin == end)
				throw std::logic_error(
				    path + ".*: non-existent style parameter(s)");

			while (begin != end)
				begin++->second.fromString(params, value);
		}
		else
			throw std::logic_error(
			    path + "/" + value
			    + ": non-existent style parameter");
	}

	static std::string getParam(Params &params,
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
