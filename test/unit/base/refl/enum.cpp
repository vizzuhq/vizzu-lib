#include "base/refl/enum.h"

#include "../../util/test.h"

using namespace test;

namespace Foo
{
class Enum(fobar)(foo,bar);
class Enum(fobar2)(foo,bar);

struct Foobar
{
	class Enum(fobar)(foo,bar);
};
}

namespace Bar
{
template <typename T> std::string toString(T v)
{
	return std::string(v);
}
template <typename T> T parse(std::string s) { return T(s); }
}

static auto tests =
    collection::add_suite("Refl::Enum")

        .add_case("enum_count_is_available_compile_time",
            []
            {
	            static_assert(Foo::fobar::EnumInfo::count() == 2u,
	                "");
            })

        .add_case("enum_names_are_available_compile_time",
            []
            {
	            static_assert(Foo::fobar::EnumInfo::names[0] == "foo",
	                "");
	            static_assert(Foo::fobar::EnumInfo::names[1] == "bar",
	                "");
            })

        .add_case("enum_names_are_available_run_time",
            []
            {
	            check() << Foo::fobar::EnumInfo::names[0] == "foo";
	            check() << Foo::fobar::EnumInfo::names[1] == "bar";
            })

        .add_case("enum_can_be_converted_to_string",
            []
            {
	            check() << (std::string)Foo::fobar(Foo::fobar::foo)
	                == "foo";
	            check() << (std::string)Foo::fobar(Foo::fobar::bar)
	                == "bar";
            })

        .add_case("enum_can_be_created_from_string",
            []
            {
	            check() << Foo::fobar("foo") == Foo::fobar::foo;
	            check() << Foo::fobar("bar") == Foo::fobar::bar;
            })

        .add_case("invalid_enum_to_string_throws",
            []
            {
	            throws<std::logic_error>() << []
	            {
		            return (std::string)Foo::fobar(
		                Foo::fobar::EnumType{2});
	            };
            })

        .add_case("enum_from_unknown_string_throws",
            []
            {
	            throws<std::logic_error>() << []
	            {
		            return Foo::fobar("baz");
	            };
            })

        .add_case("reflector_can_be_used_in_other_namespace",
            []
            {
	            check() << Bar::toString(Foo::fobar(Foo::fobar::bar))
	                == "bar";
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
