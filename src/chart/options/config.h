#ifndef CHART_CONFIG_H
#define CHART_CONFIG_H

#include <functional>
#include <string>
#include <utility>

#include "options.h"

namespace Vizzu::Gen
{

class Config
{
public:
	static std::string paramsJson();
	[[nodiscard]] std::string getParam(const std::string &path) const;
	void setParam(const std::string &path, const std::string &value);
	explicit Config(Options &options, Data::DataTable &table);

private:
	std::reference_wrapper<Options> options;
	std::reference_wrapper<Data::DataTable> table;

	void setChannelParam(const std::string &path,
	    const std::string &value);
	[[nodiscard]] std::string getChannelParam(
	    const std::string &path) const;
};

}

#endif
