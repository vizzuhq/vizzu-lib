#include "base/text/funcstring.h"

#include "../../util/test.h"

using namespace test;
using namespace Text;

static auto tests =
    collection::add_suite("Text::FuncString")

        .add_case("empty_string_results_in_empty_object",
            []
            {
	            const FuncString f("");
	            check() << f.isEmpty();
            })

        .add_case("whitespace_string_results_in_empty_object",
            []
            {
	            const FuncString f("   ");
	            check() << f.isEmpty();
            })

        .add_case("function_with_multiple_parameter_parsed",
            []
            {
	            const FuncString f("foo(bar,baz)");
	            check() << f.isEmpty() == false;
	            check() << f.getName() == "foo";
	            check() << f.getParams().size() == 2U;
	            check() << f.getParams().at(0) == "bar";
	            check() << f.getParams().at(1) == "baz";
            })

        .add_case("function_with_additional_spaces_parsed",
            []
            {
	            const FuncString f("   foo  (  bar   ,  baz  )");
	            check() << f.isEmpty() == false;
	            check() << f.getName() == "foo";
	            check() << f.getParams().size() == 2U;
	            check() << f.getParams().at(0) == "bar";
	            check() << f.getParams().at(1) == "baz";
            })

        .add_case("function_with_one_parameter_parsed",
            []
            {
	            const FuncString f("foo(bar)");
	            check() << f.isEmpty() == false;
	            check() << f.getName() == "foo";
	            check() << f.getParams().size() == 1U;
	            check() << f.getParams().at(0) == "bar";
            })

        .add_case("function_without_parameter_parsed",
            []
            {
	            const FuncString f("foo()");
	            check() << f.isEmpty() == false;
	            check() << f.getName() == "foo";
	            check() << f.getParams().size() == 0U;
            })

        .add_case("function_with_missing_parameter_parsed",
            []
            {
	            const FuncString f("foo(bar,,baz)");
	            check() << f.isEmpty() == false;
	            check() << f.getName() == "foo";
	            check() << f.getParams().size() == 3U;
	            check() << f.getParams().at(0) == "bar";
	            check() << f.getParams().at(1) == "";
	            check() << f.getParams().at(2) == "baz";
            })

        .add_case("throws_on_not_matching_string",
            []
            {
	            throws<std::exception>() << []
	            {
		            FuncString("foo");
	            };
            })

    ;
