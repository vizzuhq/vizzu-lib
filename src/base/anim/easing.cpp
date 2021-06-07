#include "easing.h"

#include "base/conv/parse.h"
#include "base/text/smartstring.h"
#include "base/text/funcstring.h"
#include "base/anim/easingfunc.h"
#include "base/anim/easinggradient.h"

using namespace Anim;
using namespace Conv;

Easing::Easing(const std::string &name)
{
	if (name.empty()) return;

	auto nameCopy = name;
	Text::SmartString::trim(nameCopy);

	if (nameCopy == "none")
	{
		func = &Easing::none;
	}
	else if (nameCopy == "linear")
	{
		func = &Easing::linear;
	}
	else if (nameCopy == "step-start")
	{
		func = &Easing::start;
	}
	else if (nameCopy == "step-end")
	{
		func = &Easing::end;
	}
	else if (nameCopy == "ease")
	{
		func = &EaseFunc::inOut<&EaseFunc::sine>;
	}
	else if (nameCopy == "ease-in")
	{
		func = &EaseFunc::in<&EaseFunc::sine>;
	}
	else if (nameCopy == "ease-out")
	{
		func = &EaseFunc::out<&EaseFunc::sine>;
	}
	else if (
		Text::FuncString f(nameCopy, false);
		f.getName() == "cubic-bezier")
	{
		if (f.getParams().size() != 4)
			throw std::logic_error("parameter count missmatch");

		Geom::Point p1(
			parse<double>(f.getParams().at(0)),
			parse<double>(f.getParams().at(1)));

		Geom::Point p2(
			parse<double>(f.getParams().at(2)),
			parse<double>(f.getParams().at(3)));

		func = EasingGradient::Bezier(p1, p2);
	}
	else throw std::logic_error("invalid easing value");
}
