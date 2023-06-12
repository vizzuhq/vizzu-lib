#include "base/conv/numtostr.h"

#include "../../util/test.h"

using namespace test;
using namespace Conv;

static auto tests =
    collection::add_suite("Conv::NumberToString")

        .add_case("NumberToString default conversion",
            []
            {
	            NumberToString converter;
	            converter.fractionDigitCount = 9;

	            check() << converter(1.00000000005) == "1";
	            check() << converter(1.0000000005) == "1.000000001";
	            check() << converter(1.000000005) == "1.000000005";
	            check() << converter(1.5) == "1.5";
	            check() << converter(0) == "0";
	            check() << converter(1) == "1";
	            check() << converter(42) == "42";
	            check() << converter(-42) == "-42";
	            check() << converter(42.42) == "42.42";
	            check() << converter(-42.42) == "-42.42";
	            check() << converter(42000.4200024)
	                == "42000.4200024";
	            check() << converter(-42000.4200024)
	                == "-42000.4200024";
	            check() << converter(42.00042) == "42.00042";
	            check() << converter(0.000000042) == "0.000000042";
	            check() << converter(0.1) == "0.1";
	            check() << converter(0.12) == "0.12";
	            check() << converter(0.123) == "0.123";
	            check() << converter(0.1234) == "0.1234";
	            check() << converter(0.12345) == "0.12345";
	            check() << converter(0.123456) == "0.123456";
	            check() << converter(0.1234567) == "0.1234567";
	            check() << converter(0.12345678) == "0.12345678";
	            check() << converter(0.123456789) == "0.123456789";
	            check() << converter(1) == "1";
	            check() << converter(12) == "12";
	            check() << converter(123) == "123";
	            check() << converter(1234) == "1234";
	            check() << converter(12345) == "12345";
	            check() << converter(123456) == "123456";
	            check() << converter(1234567) == "1234567";
	            check() << converter(12345678) == "12345678";
	            check() << converter(123456789) == "123456789";
	            check() << converter(123456789123) == "123456789123";
	            check() << converter(12345678912345678)
	                == "12345678912345678";
	            check() << converter(std::nextafter(
	                std::numeric_limits<uint64_t>::max(),
	                0))
	                == "18446744073709549568";
	            throws{} << [&]
	            {
		            check() << converter(static_cast<double>(
		                std::numeric_limits<uint64_t>::max()))
		                == "";
	            };
            })

        .add_case("NumberToString digit groupping conversion",
            []
            {
	            NumberToString converter;
	            converter.fractionDigitCount = 9;
	            converter.integerGgrouping = ',';
	            converter.fractionGgrouping = '\'';

	            check() << converter(0) == "0";
	            check() << converter(1) == "1";
	            check() << converter(42) == "42";
	            check() << converter(-42) == "-42";
	            check() << converter(42.42) == "42.42";
	            check() << converter(-42.42) == "-42.42";
	            check() << converter(42000.4200024)
	                == "42,000.420'002'4";
	            check() << converter(-42000.4200024)
	                == "-42,000.420'002'4";
	            check() << converter(42.00042) == "42.000'42";
	            check() << converter(0.000000042) == "0.000'000'042";
	            check() << converter(0.1) == "0.1";
	            check() << converter(0.12) == "0.12";
	            check() << converter(0.123) == "0.123";
	            check() << converter(0.1234) == "0.123'4";
	            check() << converter(0.12345) == "0.123'45";
	            check() << converter(0.123456) == "0.123'456";
	            check() << converter(0.1234567) == "0.123'456'7";
	            check() << converter(0.12345678) == "0.123'456'78";
	            check() << converter(0.123456789) == "0.123'456'789";
	            check() << converter(1) == "1";
	            check() << converter(12) == "12";
	            check() << converter(123) == "123";
	            check() << converter(1234) == "1,234";
	            check() << converter(12345) == "12,345";
	            check() << converter(123456) == "123,456";
	            check() << converter(1234567) == "1,234,567";
	            check() << converter(12345678) == "12,345,678";
	            check() << converter(123456789) == "123,456,789";
	            check() << converter(123456789123)
	                == "123,456,789,123";
            })

        .add_case("NumberToString static fraction length conversion",
            []
            {
	            NumberToString converter;
	            converter.fractionDigitCount = 9;
	            converter.fillFractionWithZero = true;

	            check() << converter(42000.4200024)
	                == "42000.420002400";
	            check() << converter(-42000.4200024)
	                == "-42000.420002400";
	            check() << converter(42.00042) == "42.000420000";
	            check() << converter(0.000000042) == "0.000000042";
	            check() << converter(0.1) == "0.100000000";
	            check() << converter(0.12) == "0.120000000";
	            check() << converter(0.123) == "0.123000000";
	            check() << converter(0.1234) == "0.123400000";
	            check() << converter(0.12345) == "0.123450000";
	            check() << converter(0.123456) == "0.123456000";
	            check() << converter(0.1234567) == "0.123456700";
	            check() << converter(0.12345678) == "0.123456780";
	            check() << converter(0.123456789) == "0.123456789";
            })

    ;
