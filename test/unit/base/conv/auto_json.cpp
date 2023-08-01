#include "base/conv/auto_json.h"

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

static auto tests =
    collection::add_suite("Conv::toJson")

        .add_case("ToJson vector test",
            []
            {
	            check() << Conv::toJson(MyObj{})
	                == R"({"mymem":[1,2,3,4,5],"oth":[[0,1],[2,3]]})";
            })
        .add_case("ToJson manual test",
            []
            {
	            std::string res;
	            {
		            Conv::Json j(res);
		            j(5, {"a"})(6, {"b", "c"})(std::vector{"a"},
		                {"b", "x"});

		            j(SimpleObj{}, {"b", "a"});
	            }
	            check() << res
	                == R"({"a":5,"b":{"c":6,"x":["a"],"a":{"o":2}}})";
            });