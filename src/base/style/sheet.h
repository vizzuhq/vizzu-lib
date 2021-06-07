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

	Params getFullParams() const {
		return activeParams
			? Style::ParamMerger<Params>(defaultParams, *activeParams).merged
			: throw std::logic_error("no active parameters set");
	}

	void setParamDefault(const std::string &path, const std::string &value) {
		setParam(defaultParams, path, value);
	}

	void setParam(const std::string &path, const std::string &value) {
		if (!activeParams)
			throw std::logic_error("no active parameters set");

		setParam(*activeParams, path, value);
	}

	bool hasParam(const std::string &path) {
		return Style::ParamRegistry<Params>::instance().hasParam(path);
	}

	static void setParam(
		Params &params,
		const std::string &path,
		const std::string &value)
	{
		Style::ParamRegistry<Params>::instance().visit(path,
		    [&](auto &p)
		    {
				p.fromString(params, value);
		    });
	}



private:
	Params defaultParams;
	Params *activeParams;
};

}

#endif
