#include "base/conv/function.h"

#include <vector>

#include "../../util/test.h"

using namespace Conv;

struct A
{
	A(int v) : v(v) {}

	[[nodiscard]] int add(int a, int b) const { return v + a + b; };

	[[nodiscard]] int weight(int a, int b) const
	{
		(void)v;
		return a + 2 * b;
	};

	void inc() { v++; }

	[[nodiscard]] int get() const { return v; }

private:
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
	            test::check() << a.get() == 3;
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
	            const std::vector<std::string> params{"3", "4"};
	            A a(2);
	            test::check() << function(&A::add)(a, params) == "9";
            })

        .add_case("throws_on_missing_params",
            []
            {
	            test::throws<std::logic_error>() << []
	            {
		            const std::vector<std::string> params{"3"};
		            A a(2);
		            return function(&A::add)(a, params);
	            };
            })

        .add_case("throws_on_too_many_params",
            []
            {
	            test::throws<std::logic_error>() << []
	            {
		            const std::vector<std::string> params{"3", "2", "1"};
		            A a(2);
		            return function(&A::add)(a, params);
	            };
            })

        .add_case("params_processed_in_correct_order",
            []
            {
	            const std::vector<std::string> params{"2", "3"};
	            A a(0);
	            test::check() << function(&A::weight)(a, params)
	                == "8";
            })

    ;
