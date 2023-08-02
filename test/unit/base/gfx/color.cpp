#include "base/gfx/color.h"

#include "../../util/test.h"

using namespace test;

static auto tests =
    collection::add_suite("Gfx::Color")

        .add_case("Convertable from hex string",
            []
            {
	            const Gfx::Color color("#12345678");
	            check() << color.red == 0x12 / 255.0;
	            check() << color.green == 0x34 / 255.0;
	            check() << color.blue == 0x56 / 255.0;
	            check() << color.alpha == 0x78 / 255.0;
            })

        .add_case("Convertable to hex string",
            []
            {
	            Gfx::Color color;
	            color.red = 0x12 / 255.0;
	            color.green = 0x34 / 255.0;
	            color.blue = 0x56 / 255.0;
	            color.alpha = 0x78 / 255.0;
	            check() << std::string(color) == "#12345678";
            })

        .add_case("Convertable to hex string",
            []
            {
	            Gfx::Color color;
	            color.red = 0x12 / 255.0;
	            color.green = 0x34 / 255.0;
	            color.blue = 0x56 / 255.0;
	            color.alpha = 0x78 / 255.0;
	            check() << std::string(color) == "#12345678";
            })

    ;
