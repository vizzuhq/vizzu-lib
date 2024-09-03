#include "colorgradient.h"

#include <cmath>
#include <stdexcept>
#include <string>

#include "base/math/segmentedfunc.h"
#include "base/math/segmentedfunc.tpp"
#include "base/text/smartstring.h"

#include "color.h"

template struct Math::SegmentedFunction<Gfx::Color,
    Gfx::ColorGradient>;

namespace Gfx
{

ColorGradient::ColorGradient(const std::string &stoplist)
{
	auto stopStrings = Text::SmartString::split(stoplist, ',', true);
	auto pos = 0.0;
	for (auto &stopString : stopStrings) {
		auto parts = Text::SmartString::split(stopString, ' ', true);
		if (parts.size() == 2) {
			pos = std::stod(parts[1]);
			stops.emplace_back(pos, Color::fromString(parts[0]));
		}
		else
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