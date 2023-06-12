#ifndef TEST_ANSI
#define TEST_ANSI

#include <string_view>

namespace test
{
namespace ansi {

static constexpr std::string_view reset{"\033[0m"};
static constexpr std::string_view fg_red{"\033[31m"};
static constexpr std::string_view fg_green{"\033[32m"};
static constexpr std::string_view fg_yellow{"\033[33m"};

}
}

#endif
