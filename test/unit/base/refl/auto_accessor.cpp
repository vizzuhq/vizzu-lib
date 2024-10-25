
#include "base/refl/auto_accessor.h"

#include "../../util/test.h"
#include "../testclasses.h"
#include "base/refl/auto_struct.h"
#include "base/style/impl.tpp"

using test::check;
using test::collection;

namespace Refl::Access
{
template <>
const std::map<std::string, Accessor<Fobar, true>, std::less<>> &
getAccessors<Fobar, true>()
{
	return Style::getAccessors<Fobar>();
}
}

const static auto tests =
    collection::add_suite("Style::ParamRegistry")

        .add_case("nested_param_can_be_get_as_string",
            []
            {
	            auto fobar = Fobar{{1, 2}, {5, 6}};

	            auto getter =
	                Refl::Access::getAccessor<Fobar, true>("foo.bar")
	                    .get;
	            check() << getter != nullptr;

	            auto foo_bar = std::stod(getter(fobar));

	            check() << foo_bar == 2;
            })

        .add_case("nested_param_can_be_set_with_string",
            []
            {
	            auto fobar = Fobar{{1, 2}, {5, 6}};

	            auto setter =
	                Refl::Access::getAccessor<Fobar, true>("foo.bar")
	                    .set;
	            check() << setter != nullptr;

	            setter(fobar, "9");

	            check() << fobar.foo.bar == 9;
            })

        .add_case("all_nested_param_can_iterated_over",
            []
            {
	            auto fobar = Fobar{{1, 2}, {5, 6}};

	            double sum = 0;

	            for (const auto &e :
	                Refl::Access::getAccessors<Fobar, true>())
		            sum += std::stod(e.second.get(fobar));

	            check() << sum == 1 + 2 + 5 + 6;
            })

        .add_case("all_nested_param_path_can_be_iterated_over",
            []
            {
	            std::string nameList;

	            for (const auto &e :
	                Refl::Access::getAccessors<Fobar, true>())
		            nameList += ":" + std::string{e.first};

	            check() << nameList
	                == ":baz.baz:baz.fobar:foo.bar:foo.foo";
            })

    ;
