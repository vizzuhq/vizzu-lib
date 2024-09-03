#include "colorgradient.h"

#include <cmath>
#include <stdexcept>
#include <string>

#include "base/text/smartstring.h"

#include "color.h"

namespace Gfx
{

ColorGradient::ColorGradient(const std::string &stoplist)
{
	for (const auto &stopString :
	    Text::SmartString::split(stoplist, ',', true)) {
		if (auto &&parts =
		        Text::SmartString::split(stopString, ' ', true);
		    parts.size() == 2)
			if (auto pos = std::stod(parts[1]);
			    std::isfinite(pos)
			    && (stops.empty() || pos >= stops.back().pos)) {
				stops.emplace_back(pos, Color::fromString(parts[0]));
				continue;
			}
		throw std::logic_error(
		    "invalid gradient stop: " + stopString);
	}
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
	ColorGradient res;
	res.stops.emplace_back(0.0 / 4.0, Gfx::Color(0.0, 0.0, 1.0));
	res.stops.emplace_back(1.0 / 4.0, Gfx::Color(0.0, 1.0, 1.0));
	res.stops.emplace_back(2.0 / 4.0, Gfx::Color(0.0, 1.0, 0.0));
	res.stops.emplace_back(3.0 / 4.0, Gfx::Color(1.0, 1.0, 0.0));
	res.stops.emplace_back(4.0 / 4.0, Gfx::Color(1.0, 0.0, 0.0));
	return res;
}

ColorGradient ColorGradient::HeatMap7Color()
{
	ColorGradient res;
	res.stops.emplace_back(0.0 / 6.0, Gfx::Color(0.0, 0.0, 0.0));
	res.stops.emplace_back(1.0 / 6.0, Gfx::Color(0.0, 0.0, 1.0));
	res.stops.emplace_back(2.0 / 6.0, Gfx::Color(0.0, 1.0, 1.0));
	res.stops.emplace_back(3.0 / 6.0, Gfx::Color(0.0, 1.0, 0.0));
	res.stops.emplace_back(4.0 / 6.0, Gfx::Color(1.0, 1.0, 0.0));
	res.stops.emplace_back(5.0 / 6.0, Gfx::Color(1.0, 0.0, 0.0));
	res.stops.emplace_back(6.0 / 6.0, Gfx::Color(1.0, 1.0, 1.0));
	return res;
}

}