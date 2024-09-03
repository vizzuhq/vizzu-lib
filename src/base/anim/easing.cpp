#include "easing.h"

#include <stdexcept>
#include <string>

#include "base/anim/easinggradient.h"
#include "base/conv/parse.h"
#include "base/geom/point.h"
#include "base/text/funcstring.h"
#include "base/text/smartstring.h"

namespace Anim
{

Easing::Easing(const std::string &name)
{
	if (name.empty()) return;

	auto nameCopy = name;
	Text::SmartString::trim(nameCopy);

	if (nameCopy == "none") { func = &Easing::none; }
	else if (nameCopy == "linear") {
		func = &Easing::linear;
	}
	else if (nameCopy == "step-start") {
		func = &Easing::start;
	}
	else if (nameCopy == "step-end") {
		func = &Easing::end;
	}
	else if (nameCopy == "ease") {
		func = EasingGradient::Bezier(Geom::Point{0.25, 0.1},
		    Geom::Point{0.25, 1});
	}
	else if (nameCopy == "ease-in") {
		func = EasingGradient::Bezier(Geom::Point{0.42, 0},
		    Geom::Point{1, 1});
	}
	else if (nameCopy == "ease-out") {
		func = EasingGradient::Bezier(Geom::Point{0, 0},
		    Geom::Point{0.58, 1});
	}
	else if (nameCopy == "ease-in-out") {
		func = EasingGradient::Bezier(Geom::Point{0.42, 0},
		    Geom::Point{0.58, 1});
	}
	else if (const Text::FuncString f(nameCopy);
	         f.getName() == "cubic-bezier") {
		if (f.getParams().size() != 4)
			throw std::logic_error("parameter count missmatch");

		using Conv::parse;

		const Geom::Point p1(parse<double>(f.getParams().at(0)),
		    parse<double>(f.getParams().at(1)));

		const Geom::Point p2(parse<double>(f.getParams().at(2)),
		    parse<double>(f.getParams().at(3)));

		func = EasingGradient::Bezier(p1, p2);
	}
	else
		throw std::logic_error("invalid easing value");
}

}