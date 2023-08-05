#include "base/conv/auto_json.h"

#include <map>

#include "../../util/test.h"

using namespace test;

struct MyObj
{
	std::vector<int> mymem{1, 2, 3, 4, 5};
	std::vector<std::vector<int>> oth{{0, 1}, {2, 3}};
};

struct SimpleObj
{
	int o{2};
};

struct ComplicatedObj : SimpleObj
{
	struct
	{
		SimpleObj i{4};
		ComplicatedObj *ptr{};

		[[nodiscard]] std::string toJSON() const
		{
			std::string res;
			Conv::JSONObj{res}(i, "o")(ptr, "ptr");
			return res;
		}
	} x;

	consteval static auto members()
	{
		return std::tuple(&ComplicatedObj::x);
	}
};

static auto tests =
    collection::add_suite("Conv::toJSON")
        .add_case("ToJSON primitive",
            []
            {
	            check() << Conv::toJSON(42) == "42";
	            check() << Conv::toJSON(0.42) == "0.420000";
	            check() << Conv::toJSON(true) == "true";
	            check() << Conv::toJSON(std::optional<int>{})
	                == "null";
	            check() << Conv::toJSON(
	                std::optional<std::string>{"42"})
	                == "\"42\"";
	            check() << Conv::toJSON(std::string_view{"HELLO"})
	                == "\"HELLO\"";
            })
        .add_case("ToJSON array",
            []
            {
	            check() << Conv::toJSON<std::initializer_list<int>>(
	                {1, 2, 3, 4})
	                == "[1,2,3,4]";
	            check() << Conv::toJSON(std::vector{"a", "1", "b"})
	                == R"(["a","1","b"])";

	            check() << Conv::toJSON(std::tuple{"a", 1, "b"})
	                == R"(["a",1,"b"])";
            })
        .add_case("ToJSON object",
            []
            {
	            check() << Conv::toJSON(
	                std::map<std::string_view, int>{{"a", 1},
	                    {"b", 23}})
	                == R"({"a":1,"b":23})";
            })
        .add_case("ToJSON vector test",
            []
            {
	            check() << Conv::toJSON(MyObj{})
	                == R"({"mymem":[1,2,3,4,5],"oth":[[0,1],[2,3]]})";
            })
        .add_case("ToJSON nested obj",
            []
            {
	            check() << Conv::toJSON(ComplicatedObj{})
	                == R"({"o":2,"x":{"o":{"o":4},"ptr":null}})";
            })
        .add_case("ToJson manual test",
            []
            {
	            std::string res;
	            {
		            Conv::JSONObj j(res);
		            j(5, "a");

		            j.closeOpenObj({"b"});
		            {
		                Conv::JSONObj b{res};
			            b(6, "c")(std::vector{"a"}, "x");
			            b(SimpleObj{}, "a");

		            }
	            }
	            check() << res
	                == R"({"a":5,"b":{"c":6,"x":["a"],"a":{"o":2}}})";
            });