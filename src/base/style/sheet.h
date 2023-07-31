#ifndef STYLE_STYLE_H
#define STYLE_STYLE_H

#include <stdexcept>
#include <string>
#include <vector>

#include "base/style/parammerger.h"
#include "base/style/paramregistry.h"

namespace Style
{

template <class Params> class Sheet
{
public:
	Sheet(Params defaultParams) :
	    defaultParams(std::move(defaultParams)),
	    activeParams(nullptr)
	{}

	void setActiveParams(Params &params) { activeParams = &params; }

	[[nodiscard]] const Params &getDefaultParams() const
	{
		return defaultParams;
	}

	[[nodiscard]] Params getFullParams() const
	{
		return activeParams
		         ? Style::ParamMerger<Params>(defaultParams,
		             *activeParams)
		               .merged
		         : throw std::logic_error("no active parameters set");
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
		if (hasParam(path)) {
			Style::ParamRegistry<Params>::instance().visit(path,
			    [&](auto &p)
			    {
				    p.fromString(params, value);
			    });
		}
		else if (value == "null") {
			auto closedPath = path.empty() ? path : path + ".";

			auto count =
			    Style::ParamRegistry<Params>::instance().visit(
			        [&](auto &p)
			        {
				        p.fromString(params, value);
			        },
			        closedPath);

			if (count == 0)
				throw std::logic_error(
				    path + ".*: non-existent style parameter(s)");
		}
		else
			throw std::logic_error(
			    path + "/" + value
			    + ": non-existent style parameter");
	}

	static std::string getParam(Params &params,
	    const std::string &path)
	{
		if (!hasParam(path))
			throw std::logic_error(
			    path + ": non-existent style parameter");

		std::string res;
		Style::ParamRegistry<Params>::instance().visit(path,
		    [&](auto &p)
		    {
			    res = p.toString(params);
		    });
		return res;
	}

protected:
	Params defaultParams;
	Params *activeParams;
};

}

#endif
