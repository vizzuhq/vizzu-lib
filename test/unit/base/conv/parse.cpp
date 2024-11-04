#include "base/conv/parse.h"

#include "../../util/test.h"

using Conv::parse;
using test::check;
using test::collection;
using test::throws;

const static auto tests =
    collection::add_suite("Conv::parse")

        .add_case("parses_string",
            []
            {
	            check() << parse<std::string>("foo") == "foo";
            })

        .add_case("parses_type_with_string_constructor",
            []
            {
	            struct foo
	            {
		            std::string string;
	            };
	            check() << parse<foo>("foo").string == "foo";
            })

        .add_case("parses_streamable_type",
            []
            {
	            check() << parse<int>("42") == 42;
            })

        .add_case("parses_boolalpha_to_bool",
            []
            {
	            check() << parse<bool>("true") == true;
	            check() << parse<bool>("false") == false;
            })

        .add_case("throws_on_invalid_boolalpha",
            []
            {
	            throws<std::bad_cast>() << []
	            {
		            check() << parse<bool>("foo") == true;
	            };
            })

    ;
