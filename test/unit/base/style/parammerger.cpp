#include "base/style/parammerger.h"

#include "../../util/test.h"
#include "../testclasses.h"

using test::check;
using test::collection;

const static auto tests =
    collection::add_suite("Style::ParamMerger")

        .add_case("overwrites_set_parameters",
            []
            {
	            const Fobar base{{1, 2}, {5, 6}};
	            Fobar over;
	            over.foo.bar = 9;

	            auto merged = Style::ParamMerger::merge(base, over);

	            check() << merged.foo.bar == 9;
            })

        .add_case("does_not write_over_unset_parameters",
            []
            {
	            const Fobar base{{1, 2}, {5, 6}};
	            Fobar over;
	            over.foo.bar = 9;

	            auto merged = Style::ParamMerger::merge(base, over);

	            check() << merged.foo.foo == 1;
	            check() << merged.baz.baz == 5;
	            check() << merged.baz.fobar == 6;
            })

    ;
