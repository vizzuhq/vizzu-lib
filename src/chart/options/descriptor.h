#ifndef CHART_DESCRIPTOR_H
#define CHART_DESCRIPTOR_H

#include <map>
#include <string>
#include <functional>

#include "base/refl/enum.h"
#include "chart/options/optionssetter.h"

namespace Vizzu
{
namespace Diag
{

class Descriptor {
public:
	class Enum(CoordSystem)(cartesian, polar);
	class Enum(Direction)(upright, left, upsideDown, right);
	class Enum(Geometry)(rectangle, circle, area, line);
	class Enum(Orientation)(horizontal, vertical);
	class Enum(Sort)(none, experimental);
	class Enum(Align)(none, min, center, max, stretch);

	void setParam(const std::string &path, const std::string &value);
	Descriptor(OptionsSetterPtr setter) : setter(setter) {}

private:
	typedef std::function<void(OptionsSetter&, const std::string&)>
	    Accessor;

	typedef std::map<std::string, Accessor> Accessors;

	const static Accessors accessors;
	OptionsSetterPtr setter;

	static Accessors initAccessors();

	void setChannelParam(const std::string &path, const std::string &value);
	void setFilter(const std::string &path, const std::string &value);
};

}
}

#endif
