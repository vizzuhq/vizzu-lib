#include "colorgradient.h"

#include <cmath>
#include <stdexcept>
#include <string>

#include "base/text/smartstring.h"

#include "color.h"

namespace Gfx
{

ColorGradient ColorGradient::fromString(const std::string &stoplist)
{
	ColorGradient res;
	for (const auto &stopString :
	    Text::SmartString::split(stoplist, ',', true)) {
		if (auto &&parts =
		        Text::SmartString::split(stopString, ' ', true);
		    parts.size() == 2)
			if (auto pos = std::stod(parts[1]);
			    std::isfinite(pos)
			    && (res.stops.empty()
			        || pos >= res.stops.back().pos)) {
				res.stops.emplace_back(pos,
				    Color::fromString(parts[0]));
				continue;
			}
		throw std::logic_error(
		    "invalid gradient stop: " + stopString);
	}
	return res;
}

ColorGradient::operator std::string() const
{
	std::string res;
	for (const auto &stop : stops) {
		if (!res.empty()) res += ", ";
		res += static_cast<std::string>(stop.value) + " "
		     + std::to_string(stop.pos);
	}
	return res;
}

ColorGradient ColorGradient::HeatMap5Color()
{
	return {{0.0 / 4.0, Color(0.0, 0.0, 1.0)},
	    {1.0 / 4.0, Color(0.0, 1.0, 1.0)},
	    {2.0 / 4.0, Color(0.0, 1.0, 0.0)},
	    {3.0 / 4.0, Color(1.0, 1.0, 0.0)},
	    {4.0 / 4.0, Color(1.0, 0.0, 0.0)}};
}

ColorGradient ColorGradient::HeatMap7Color()
{
	return {{0.0 / 6.0, Color(0.0, 0.0, 0.0)},
	    {1.0 / 6.0, Color(0.0, 0.0, 1.0)},
	    {2.0 / 6.0, Color(0.0, 1.0, 1.0)},
	    {3.0 / 6.0, Color(0.0, 1.0, 0.0)},
	    {4.0 / 6.0, Color(1.0, 1.0, 0.0)},
	    {5.0 / 6.0, Color(1.0, 0.0, 0.0)},
	    {6.0 / 6.0, Color(1.0, 1.0, 1.0)}};
}

}