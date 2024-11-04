#include "base/text/funcstring.h"

#include "../../util/test.h"
#include "base/conv/parse.h"

using test::check;
using test::collection;
using test::throws;
using Text::FuncString;

const static auto tests =
    collection::add_suite("Text::FuncString")

        .add_case("empty_string_results_in_empty_object",
            []
            {
	            const FuncString f("");
	            check() << f.isEmpty() == true;
            })

        .add_case("whitespace_string_results_in_empty_object",
            []
            {
	            const FuncString f("   ");
	            check() << f.isEmpty() == true;
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

        .add_case("function_with_space_in_parameters",
            []
            {
	            const FuncString f("foo( )");
	            check() << f.isEmpty() == false;
	            check() << f.getName() == "foo";
	            check() << f.getParams().size() == 0U;
            })

        .add_case("function_with_parameter_contains_parenthesis",
            []
            {
	            const FuncString f("foo(bar())");
	            check() << f.isEmpty() == false;
	            check() << f.getName() == "foo";
	            check() << f.getParams().size() == 1U;
	            check() << f.getParams().at(0) == "bar()";
            })

        .add_case("throws_on_not_matching_string",
            []
            {
	            throws<std::exception>() << []
	            {
		            check() << Conv::parse<FuncString>("foo")
		                           .isEmpty()
		                == 0.0;
	            };
            })

    ;
