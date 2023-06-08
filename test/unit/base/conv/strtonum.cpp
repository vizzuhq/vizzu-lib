#include "base/conv/strtonum.h"

#include "../../util/test.h"

using namespace test;
using namespace Conv;

static auto tests = collection::add_suite("Conv::StringToNumber")

.add_case( "StringToNumber default conversions",
[]{
	StringToNumber converter;

	check() << converter.convertInteger("0") == 0;
	check() << converter.convertInteger("00000") == 0;
	check() << converter.convertFloatingPoint("0") == 0;
	check() << converter.convertFloatingPoint("0.0") == 0;
	check() << converter.convertFloatingPoint("0000.00000") == 0;
	check() << converter.convertInteger("1") == 1;
	check() << converter.convertFloatingPoint("1") == 1;
	check() << converter.convertFloatingPoint("1.0") == 1;
	check() << converter.convertInteger("424242") == 424242;
	check() << converter.convertFloatingPoint("424242.0") == 424242;
	check() << converter.convertFloatingPoint("-0") == 0;
	check() << converter.convertFloatingPoint("+0") == 0;
	check() << converter.convertFloatingPoint("-1") == -1;
	check() << converter.convertFloatingPoint("+1") == 1;
	check() << converter.convertInteger("-0") == 0;
	check() << converter.convertInteger("+0") == 0;
	check() << converter.convertInteger("-1") == -1;
	check() << converter.convertInteger("+1") == 1;
	check() <<
		(0.123455 < converter.convertFloatingPoint("0.123456") &&
		0.123457 > converter.convertFloatingPoint("0.123456"));
	check() << converter.convertFloatingPoint("42.42") == 42.42;
	check() << converter.convertFloatingPoint("42.0000042") == 42.0000042;
})

;
