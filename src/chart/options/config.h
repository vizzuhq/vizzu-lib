#ifndef CHART_CONFIG_H
#define CHART_CONFIG_H

#include <functional>
#include <list>
#include <map>
#include <string>
#include <utility>

#include "chart/options/optionssetter.h"

namespace Vizzu::Gen
{

class Config
{
public:
	enum class CoordSystem { cartesian, polar };
	enum class Orientation { horizontal, vertical };
	enum class Sort { none, byValue };

	static std::list<std::string> listParams();
	[[nodiscard]] std::string getParam(const std::string &path) const;
	void setParam(const std::string &path, const std::string &value);
	void setFilter(Data::Filter::Function &&func, uint64_t hash);
	Config(OptionsSetterPtr setter) : setter(std::move(setter)) {}

private:
	struct Accessor
	{
		std::function<std::string(const Options &)> get;
		std::function<void(OptionsSetter &, const std::string &)> set;
	};

	using Accessors = std::map<std::string, Accessor>;

	const static Accessors accessors;
	OptionsSetterPtr setter;

	static Accessors initAccessors();

	void setChannelParam(const std::string &path,
	    const std::string &value);
	[[nodiscard]] std::string getChannelParam(
	    const std::string &path) const;
	static std::list<std::string> listChannelParams();
};

}

#endif
