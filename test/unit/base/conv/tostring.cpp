#include "base/conv/tostring.h"

#include "../../util/test.h"

using namespace test;
using namespace Conv;

const static auto tests =
    collection::add_suite("Conv::toString")

        .add_case("converts_const_char_pointer",
            []
            {
	            check() << toString("foo") == "foo";
            })

        .add_case("converts_string",
            []
            {
	            check() << toString(std::string("foo")) == "foo";
            })

        .add_case("converts_type_with_string_operator",
            []
            {
	            struct
	            {
		            explicit operator std::string() const
		            {
			            return "foo";
		            }
	            } const obj;

	            check() << toString(obj) == "foo";
            })

        .add_case("converts_streamable_type",
            []
            {
	            check() << toString(42) == "42";
            })

        .add_case("converts_bool_to_boolalpha",
            []
            {
	            check() << toString(true) == "true";
	            check() << toString(false) == "false";
            })

    ;
