#include "base/style/paramregistry.h"

#include "../../util/test.h"

#include "teststyle.h"

using namespace test;

static auto tests = collection::add_suite("Style::ParamRegistry")

.add_case("nested_param_can_be_get_as_string",
[]{
	Fobar fobar{ { 1, 2 }, { 5, 6 } };

	double foo_bar;

	Style::ParamRegistry<Fobar>::instance().visit("foo.bar",
	    [&](auto &p)
	    {
		    foo_bar = std::stod(p.toString(fobar));
	    });

	check() << foo_bar == 2;
})


.add_case("nested_param_can_be_set_with_string",
[]{
	Fobar fobar{ { 1, 2 }, { 5, 6 } };

	Style::ParamRegistry<Fobar>::instance().visit("foo.bar",
	    [&](auto &p)
	    {
		    p.fromString(fobar, "9");
	    });

	check() << fobar.foo.bar == 9;
})


.add_case("all_nested_param_can_iterated_over",
[]{
	Fobar fobar{ { 1, 2 }, { 5, 6 } };

	double sum = 0;

	Style::ParamRegistry<Fobar>::instance().visit(
	    [&](auto &p)
	    {
		    sum += std::stod(p.second->toString(fobar));
	    });

	check() << sum == 1+2+5+6;
})


.add_case("all_nested_param_path_can_be_iterated_over",
[]{
	std::string nameList;

	Style::ParamRegistry<Fobar>::instance().visit(
	    [&](auto &p)
	    {
		    nameList += ":" + p.first;
	    });

	check() << nameList == ":baz.baz:baz.fobar:foo.bar:foo.foo";
})

;
