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
	static std::list<std::string> listParams();
	[[nodiscard]] std::string getParam(const std::string &path) const;
	void setParam(const std::string &path, const std::string &value);
	void setFilter(Data::Filter::Function &&func, uint64_t hash);
	explicit Config(OptionsSetterPtr setter) :
	    setter(std::move(setter))
	{}

private:
	struct Accessor
	{
		std::string (*get)(const Options &);
		void (*set)(OptionsSetter &, const std::string &);
	};

	template <auto Mptr, auto Set>
	static const std::pair<std::string_view, Config::Accessor>
	    accessor;

	using Accessors = std::map<std::string_view, Accessor>;

	static const Accessors &getAccessors();
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
