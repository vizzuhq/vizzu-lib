#include "base/style/paramregistry.h"

#include "../../util/test.h"
#include "base/refl/auto_struct.h"
#include "base/style/impl.tpp"

#include "teststyle.h"

using namespace test;

template Style::ParamRegistry<Fobar>::ParamRegistry();

const static auto tests =
    collection::add_suite("Style::ParamRegistry")

        .add_case("nested_param_can_be_get_as_string",
            []
            {
	            auto fobar = Fobar{{1, 2}, {5, 6}};

	            auto foo_bar =
	                std::stod(Style::ParamRegistry<Fobar>::instance()
	                              .find("foo.bar")
	                              ->toString(fobar));

	            check() << foo_bar == 2;
            })

        .add_case("nested_param_can_be_set_with_string",
            []
            {
	            auto fobar = Fobar{{1, 2}, {5, 6}};

	            Style::ParamRegistry<Fobar>::instance()
	                .find("foo.bar")
	                ->fromString(fobar, "9");

	            check() << fobar.foo.bar == 9;
            })

        .add_case("all_nested_param_can_iterated_over",
            []
            {
	            auto fobar = Fobar{{1, 2}, {5, 6}};

	            double sum = 0;

	            for (auto &e : Style::ParamRegistry<Fobar>::instance()
	                               .prefix_range(""))
		            sum += std::stod(e.second.toString(fobar));

	            check() << sum == 1 + 2 + 5 + 6;
            })

        .add_case("all_nested_param_path_can_be_iterated_over",
            []
            {
	            std::string nameList;

	            for (auto &e : Style::ParamRegistry<Fobar>::instance()
	                               .prefix_range(""))
		            nameList += ":" + e.first;

	            check() << nameList
	                == ":baz.baz:baz.fobar:foo.bar:foo.foo";
            })

    ;
