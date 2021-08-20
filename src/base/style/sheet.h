#ifndef STYLE_STYLE_H
#define STYLE_STYLE_H

#include <stdexcept>
#include <string>
#include <vector>

#include "base/style/parammerger.h"
#include "base/style/paramregistry.h"

namespace Style
{

template <class Params>
class Sheet
{
public:
	Sheet(const Params &defaultParams) :
	    defaultParams(defaultParams),
	    activeParams(nullptr)
	{}

	void setActiveParams(Params &params) {
		activeParams = &params;
	}

	const Params &getDefaultParams() const { return defaultParams; }

	Params getFullParams() const {
		return activeParams
			? Style::ParamMerger<Params>(defaultParams, *activeParams).merged
			: throw std::logic_error("no active parameters set");
	}

	template <typename T>
	const T& 
	getFullParam(const std::function<const T&(const Params&)> get) const
	{
		if (!activeParams) 
			throw std::logic_error("no active parameters set");

		const auto &activeParam = get(*activeParams);
		return activeParam ? activeParam : get(defaultParams);
	}

	static std::list<std::string> paramList() {
		return Style::ParamRegistry<Params>::instance().listParams();
	}

	void setParamDefault(const std::string &path, const std::string &value) {
		setParam(defaultParams, path, value);
	}

	void setParam(const std::string &path, const std::string &value) {
		if (!activeParams)
			throw std::logic_error("no active parameters set");

		setParam(*activeParams, path, value);
	}

	static bool hasParam(const std::string &path) {
		return Style::ParamRegistry<Params>::instance().hasParam(path);
	}

	static void setParam(
		Params &params,
		const std::string &path,
		const std::string &value)
	{
		if (!hasParam(path))
			throw std::logic_error(
				"non-existent style parameter: " + std::string(path));

		Style::ParamRegistry<Params>::instance().visit(path,
		    [&](auto &p)
		    {
				p.fromString(params, value);
		    });
	}

	static std::string getParam(
		Params &params,
		const std::string &path)
	{
		if (!hasParam(path))
			throw std::logic_error(
				"non-existent style parameter: " + std::string(path));

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
