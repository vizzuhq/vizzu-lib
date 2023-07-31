
#include "base/refl/struct.h"

#include "../../util/test.h"

using namespace test;

struct Sum
{
	int sum {};

	template <typename T> Sum &operator()(T &value, const char *)
	{
		if constexpr (Refl::isReflectable<T, Sum>)
			value.visit(*this);
		else
			sum += value;
		return *this;
	}
};

struct Simple
{
	int foo;
	int bar;
	void visit(auto &visitor) { visitor(foo, "foo")(bar, "bar"); }
};

struct Nested
{
	struct Child
	{
		int bar;
		int baz;

		void visit(auto &visitor) { visitor(bar, "bar")(baz, "baz"); }
	};

	int foo;
	Child child;

	void visit(auto &visitor) { visitor(foo, "foo")(child, "child"); }
};

struct Base
{
	int bar;
	int baz;

	void visit(auto &visitor) { visitor(bar, "bar")(baz, "baz"); }
};

struct Derived : Base
{
	int foo;

	void visit(auto &visitor)
	{
		Base::visit(visitor);
		visitor(foo, "foo");
	}
};

;

static auto tests =
    collection::add_suite("Refl::Struct")

        .add_case("simle_struct_is_iterable",
            []
            {
	            Simple obj{1, 2};
	            Sum sum;
	            obj.visit(sum);
	            check() << sum.sum == 1 + 2;
            })

        .add_case("nested_struct_is_iterable",
            []
            {
	            Nested obj{5, {1, 2}};
	            Sum sum;
	            obj.visit(sum);
	            check() << sum.sum == 1 + 2 + 5;
            })

        .add_case("struct_with_base_class_is_iterable",
            []
            {
	            Derived obj{{1, 2}, 4};
	            Sum sum;
	            obj.visit(sum);
	            check() << sum.sum == 1 + 2 + 4;
            })

    ;
