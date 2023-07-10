#ifndef CHART_CONFIG_H
#define CHART_CONFIG_H

#include <functional>
#include <list>
#include <map>
#include <string>

#include "optionssetter.h"

namespace Vizzu
{
namespace Gen
{

class Config
{
public:
	enum class CoordSystem { cartesian, polar };
	enum class Sort { none, byValue };

	static std::list<std::string> listParams();
	std::string getParam(const std::string &path) const;
	void setParam(const std::string &path, const std::string &value);
	void setFilter(Data::Filter::Function &&func, uint64_t hash);
	Config(OptionsSetter&& setter) : setter(std::move(setter)) {}

	void serialize() const;

private:
	struct Accessor
	{
		std::function<std::string(const Options &)> get;
		std::function<void(OptionsSetter &, const std::string &)> set;
	};

	typedef std::map<std::string, Accessor> Accessors;

	const static Accessors accessors;
	OptionsSetter setter;

	static Accessors initAccessors();

	void setChannelParam(const std::string &path,
	    const std::string &value);
	std::string getChannelParam(const std::string &path) const;
	static std::list<std::string> listChannelParams();
};

}
}

#endif
