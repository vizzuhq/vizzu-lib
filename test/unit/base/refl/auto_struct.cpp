
#include "../../util/test.h"
#include "base/refl/auto_struct.h"

using namespace test;

template <class S> struct Sum
{
	S sum;
	Sum() : sum{} {}

	template <typename T>
		requires(requires(S s, T t){ s += t; })
	Sum &operator()(T &value)
	{
		sum += value;
		return *this;
	}
};

struct Simple
{
	int foo;
	int bar;
};

static_assert(std::is_same_v<Refl::bases_t<Simple>, std::tuple<>>);
static_assert(
    std::is_same_v<Refl::members_t<Simple>, std::tuple<int, int>>);
static_assert(Refl::is_structure_bindable_v<Simple>);
static_assert(Refl::structure_binding_size_v<Simple> == 2);

struct Nested
{
	struct Child
	{
		int bar;
		int baz;
	};

	int foo;
	Child child;
};

static_assert(std::is_same_v<Refl::bases_t<Nested>, std::tuple<>>);
static_assert(std::is_same_v<Refl::members_t<Nested>,
    std::tuple<int, Nested::Child>>);
static_assert(Refl::is_structure_bindable_v<Nested>);
static_assert(Refl::structure_binding_size_v<Nested> == 2);

static_assert(
    std::is_same_v<Refl::bases_t<Nested::Child>, std::tuple<>>);
static_assert(std::is_same_v<Refl::members_t<Nested::Child>,
    std::tuple<int, int>>);
static_assert(Refl::is_structure_bindable_v<Nested::Child>);
static_assert(Refl::structure_binding_size_v<Nested::Child> == 2);

struct EmptyBase
{
};

static_assert(
    std::is_same_v<Refl::bases_t<EmptyBase>, std::tuple<>>);
static_assert(
    std::is_same_v<Refl::members_t<EmptyBase>, std::tuple<>>);
static_assert(!Refl::is_structure_bindable_v<EmptyBase>);
static_assert(!Refl::is_reflectable_v<EmptyBase>);

struct Base : EmptyBase
{
	int bar;
	int baz;
};

static_assert(
    std::is_same_v<Refl::bases_t<Base>, std::tuple<EmptyBase>>);
static_assert(
    std::is_same_v<Refl::members_t<Base>, std::tuple<int, int>>);
static_assert(Refl::is_structure_bindable_v<Base>);
static_assert(Refl::structure_binding_size_v<Base> == 2);

struct Derived : Base
{
	int foo;

	consteval static auto members()
	{
		return std::tuple{&Derived::foo};
	}
};

static_assert(
    std::is_same_v<Refl::bases_t<Derived>, std::tuple<Base>>);
static_assert(
    std::is_same_v<Refl::members_t<Derived>, std::tuple<int>>);
static_assert(!Refl::is_structure_bindable_v<Derived>);
static_assert(Refl::is_reflectable_v<Derived>);

struct SimpleDerived : Base
{};
static_assert(
    std::is_same_v<Refl::bases_t<SimpleDerived>, std::tuple<Base>>);
static_assert(
    std::is_same_v<Refl::members_t<SimpleDerived>, std::tuple<>>);
static_assert(Refl::is_structure_bindable_v<SimpleDerived>);
static_assert(Refl::structure_binding_size_v<SimpleDerived> == 2);

struct Nontrivial
{
	std::string_view sv;
	std::string s;
	const char *cc;
};

static_assert(
    std::is_same_v<Refl::bases_t<Nontrivial>, std::tuple<>>);
static_assert(std::is_same_v<Refl::members_t<Nontrivial>,
    std::tuple<std::string_view, std::string, const char *>>);
static_assert(Refl::is_structure_bindable_v<Nontrivial>);
static_assert(Refl::structure_binding_size_v<Nontrivial> == 3);

static auto tests =
    collection::add_suite("Refl::Struct")

        .add_case("simle_struct_is_iterable",
            []
            {
	            Simple obj{1, 2};
	            Sum<int> sum;
	            Refl::visit(sum, obj);
	            check() << sum.sum == 1 + 2;
            })

        .add_case("nested_struct_is_iterable",
            []
            {
	            Nested obj{5, {1, 2}};
	            Sum<int> sum;
	            Refl::visit(sum, obj);
	            check() << sum.sum == 1 + 2 + 5;
            })

        .add_case("struct_with_base_class_is_iterable",
            []
            {
	            Derived obj{{{}, 1, 2}, 4};
	            Sum<int> sum;
	            Refl::visit(sum, obj);
	            check() << sum.sum == 1 + 2 + 4;
            })

        .add_case("struct_with_string_view",
            []
            {
	            Nontrivial obj{"o1", "o2", "o3"};
	            Sum<std::string> sum;
	            Refl::visit(sum, obj);
	            check() << sum.sum == "o1o2o3";
            })

        .add_case("struct_with_string_view_man",
            []
            {
	            Nontrivial obj{"o1", "o2", "o3"};
	            Sum<std::string> sum;
	            Refl::visit<Nontrivial>([&] <class T>(T&& ptr) {
					sum(ptr(obj));
				});
	            check() << sum.sum == "o1o2o3";
            })


    ;
