#include "base/conv/auto_json.h"

#include "../../util/test.h"

using namespace test;

struct MyObj {
	std::vector<int> mymem {1, 2, 3, 4, 5};
	std::vector<std::vector<int>> oth { {0, 1}, {2, 3} };
};

static auto tests =
    collection::add_suite("Conv::toJson")

        .add_case("ToJson vector test",
            []
            {
	            check() << Conv::toJson(MyObj{})
	             == "{\"mymem\":[1,2,3,4,5],\"oth\":[[0,1],[2,3]]}";
            })
    ;