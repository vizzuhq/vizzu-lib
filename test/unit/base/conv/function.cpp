#include "base/conv/function.h"

#include <vector>

#include "../../util/test.h"

using namespace Conv;

struct A
{
	A(int v) : v(v) {}

	int add(int a, int b) { return v + a + b; };

	int weight(int a, int b) { return a + 2 * b; };

	void inc() { v++; }

	int get() { return v; }

	int v;
};

static auto tests =
    test::collection::add_suite("Conv::function")

        .add_case("can_wrap_void_method_with_no_params",
            []
            {
	            A a(2);
	            test::check() << function(&A::inc)(a, {})
	                == std::string();
	            test::check() << a.v == 3;
            })

        .add_case("can_wrap_method_with_no_params",
            []
            {
	            A a(2);
	            test::check() << function(&A::get)(a, {}) == "2";
            })

        .add_case("can_wrap_method_with_params",
            []
            {
	            std::vector<std::string> params{"3", "4"};
	            A a(2);
	            test::check() << function(&A::add)(a, params) == "9";
            })

        .add_case("throws_on_missing_params",
            []
            {
	            test::throws<std::logic_error>() << []
	            {
		            std::vector<std::string> params{"3"};
		            A a(2);
		            return function(&A::add)(a, params);
	            };
            })

        .add_case("throws_on_too_many_params",
            []
            {
	            test::throws<std::logic_error>() << []
	            {
		            std::vector<std::string> params{"3", "2", "1"};
		            A a(2);
		            return function(&A::add)(a, params);
	            };
            })

        .add_case("params_processed_in_correct_order",
            []
            {
	            std::vector<std::string> params{"2", "3"};
	            A a(0);
	            test::check() << function(&A::weight)(a, params)
	                == "8";
            })

    ;
