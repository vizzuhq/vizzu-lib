#ifndef CHART_CONFIG_H
#define CHART_CONFIG_H

#include <functional>
#include <list>
#include <map>
#include <string>

#include "base/refl/enum.h"
#include "chart/options/optionssetter.h"

namespace Vizzu
{
namespace Diag
{

class Config
{
public:
	class Enum(CoordSystem)(cartesian, polar);
	class Enum(Geometry)(rectangle, circle, area, line);
	class Enum(Orientation)(horizontal, vertical);
	class Enum(Sort)(none, byValue);
	class Enum(Align)(none, min, center, max, stretch);

	static std::list<std::string> listParams();
	std::string getParam(const std::string &path) const;
	void setParam(const std::string &path, const std::string &value);
	void setFilter(Data::Filter::Function func, uint64_t hash);
	Config(OptionsSetterPtr setter) : setter(setter) {}

	void serialize() const;

private:
	struct Accessor
	{
		std::function<std::string(const Options &)> get;
		std::function<void(OptionsSetter &, const std::string &)> set;
	};

	typedef std::map<std::string, Accessor> Accessors;

	const static Accessors accessors;
	OptionsSetterPtr setter;

	static Accessors initAccessors();

	void setChannelParam(const std::string &path,
	    const std::string &value);
	std::string getChannelParam(const std::string &path) const;
	static std::list<std::string> listChannelParams();
};

}
}

#endif
