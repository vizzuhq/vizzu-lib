#include "../../util/test.h"
#include "base/refl/auto_enum.h"

using namespace test;

namespace Foo
{
enum class fobar { foo, bar };

enum class uniq_names { foo, bar };

consteval auto unique_enum_names(uniq_names) { return "enum1,enum2"; }

struct Foobar
{
	enum class fobar { foo, bar };

	enum ins_struct { val1, val2 };
};
consteval auto unique_enum_names(Foobar::ins_struct)
{
	return "enum1,enum2";
}
}

static_assert(!Refl::Detail::UniqueNames<Foo::fobar>);
static_assert(Refl::Detail::UniqueNames<Foo::uniq_names>);
static_assert(Refl::Detail::UniqueNames<Foo::Foobar::ins_struct>);

namespace Bar
{
template <typename T> std::string toString(T v)
{
	return Refl::enum_name(v);
}
template <typename T> T parse(std::string s)
{
	return Refl::get_enum<T>(s);
}
}

static auto tests =
    collection::add_suite("Refl::Enum")

        .add_case("enum_count_is_available_compile_time",
            []
            {
	            static_assert(std::size(Refl::enum_names<Foo::fobar>)
	                              == 2u,
	                "");
            })

        .add_case("enum_names_are_available_compile_time",
            []
            {
	            static_assert(Refl::enum_names<Foo::fobar>[0]
	                              == "foo",
	                "");
	            static_assert(Refl::enum_names<Foo::fobar>[1]
	                              == "bar",
	                "");
            })

        .add_case("enum_names_are_available_run_time",
            []
            {
	            check() << Refl::enum_names<Foo::fobar>[0] == "foo";
	            check() << Refl::enum_names<Foo::fobar>[1] == "bar";
            })

        .add_case("enum_can_be_converted_to_string",
            []
            {
	            check() << Refl::enum_name(Foo::fobar::foo) == "foo";
	            check() << Refl::enum_name(Foo::fobar::bar) == "bar";
            })

        .add_case("enum_can_be_created_from_string",
            []
            {
	            check() << Refl::get_enum<Foo::fobar>("foo")
	                == Foo::fobar::foo;
	            check() << Refl::get_enum<Foo::fobar>("bar")
	                == Foo::fobar::bar;
            })

        .add_case("enum_unique_name_can_be_converted_to_string",
            []
            {
	            check() << Refl::enum_name(Foo::uniq_names::foo)
	                == "enum1";
	            check() << Refl::enum_name(Foo::uniq_names::bar)
	                == "enum2";
            })

        .add_case("enum_unique_name_can_be_created_from_string",
            []
            {
	            check() << Refl::get_enum<Foo::uniq_names>("enum1")
	                == Foo::uniq_names::foo;
	            check() << Refl::get_enum<Foo::uniq_names>("enum2")
	                == Foo::uniq_names::bar;
            })

        .add_case("invalid_enum_to_string_throws",
            []
            {
	            throws<std::logic_error>() << []
	            {
		            return Refl::enum_name(Foo::fobar{2});
	            };
            })

        .add_case("enum_from_unknown_string_throws",
            []
            {
	            throws<std::logic_error>() << []
	            {
		            return Refl::get_enum<Foo::fobar>("baz");
	            };
            })

        .add_case("reflector_can_be_used_in_other_namespace",
            []
            {
	            check() << Bar::toString(Foo::fobar::bar) == "bar";
	            check() << Bar::parse<Foo::fobar>("bar")
	                == Foo::fobar::bar;
            })

        .add_case("reflector_can_be_used_from_class",
            []
            {
	            check() << Bar::parse<Foo::Foobar::fobar>("bar")
	                == Foo::Foobar::fobar::bar;
            })

    ;
