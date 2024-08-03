#include "base/conv/numtostr.h"

#include <cmath>

#include "../../util/test.h"

using Conv::NumberToString;
using test::check;
using test::collection;

const static auto tests =
    collection::add_suite("Conv::NumberToString")

        .add_case("NumberToString close to precision",
            []
            {
	            NumberToString converter;
	            converter.fractionDigitCount = 3;

	            check() << converter(0.2405) == "0.241";
	            check() << converter(std::nexttoward(0.2405, 1))
	                == "0.241";
	            check() << converter(std::nexttoward(0.2405, 0))
	                == "0.24";

	            check() << converter(0.6835) == "0.684";
	            check() << converter(std::nexttoward(0.6835, 1))
	                == "0.684";
	            check() << converter(std::nexttoward(0.6835, 0))
	                == "0.683";
            })

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
	            check() << converter(
	                static_cast<double>(12345678912345678))
	                == "12345678912345678";
	            check() << converter(std::nextafter(
	                std::numeric_limits<uint64_t>::max(),
	                0))
	                == "18446744073709549568";
	            check() << converter(static_cast<double>(
	                std::numeric_limits<uint64_t>::max()))
	                == "18446744073709551616";
	            check() << converter(
	                std::numeric_limits<double>::max())
	                == "179769313486231570814527423731704356798070567"
	                   "525844996598917476803157260780028538760589558"
	                   "632766878171540458953514382464234321326889464"
	                   "182768467546703537516986049910576551282076245"
	                   "490090389328944075868508455133942304583236903"
	                   "222948165808559332123348274797826204144723168"
	                   "738177180919299881250404026184124858368";

	            check() << converter(
	                std::numeric_limits<double>::quiet_NaN())
	                == "nan";
	            check() << converter(
	                std::numeric_limits<double>::infinity())
	                == "inf";
	            check() << converter(
	                -std::numeric_limits<double>::infinity())
	                == "-inf";
	            check() << converter(-0.0) == "0";
            })

        .add_case("NumberToString digit groupping conversion",
            []
            {
	            NumberToString converter;
	            converter.fractionDigitCount = 9;
	            converter.integerGgrouping = ',';
	            converter.fractionGgrouping = '\'';
	            converter.decimalPointChar = '/';

	            check() << converter(0) == "0";
	            check() << converter(1) == "1";
	            check() << converter(42) == "42";
	            check() << converter(-42) == "-42";
	            check() << converter(42.42) == "42/42";
	            check() << converter(-42.42) == "-42/42";
	            check() << converter(42000.4200024)
	                == "42,000/420'002'4";
	            check() << converter(-42000.4200024)
	                == "-42,000/420'002'4";
	            check() << converter(42.00042) == "42/000'42";
	            check() << converter(0.000000042) == "0/000'000'042";
	            check() << converter(0.1) == "0/1";
	            check() << converter(0.12) == "0/12";
	            check() << converter(0.123) == "0/123";
	            check() << converter(0.1234) == "0/123'4";
	            check() << converter(0.12345) == "0/123'45";
	            check() << converter(0.123456) == "0/123'456";
	            check() << converter(0.1234567) == "0/123'456'7";
	            check() << converter(0.12345678) == "0/123'456'78";
	            check() << converter(0.123456789) == "0/123'456'789";
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
	            check() << converter(
	                std::numeric_limits<double>::max())
	                == "179,769,313,486,231,570,814,527,423,731,704,"
	                   "356,798,070,567,525,844,996,598,917,476,803,"
	                   "157,260,780,028,538,760,589,558,632,766,878,"
	                   "171,540,458,953,514,382,464,234,321,326,889,"
	                   "464,182,768,467,546,703,537,516,986,049,910,"
	                   "576,551,282,076,245,490,090,389,328,944,075,"
	                   "868,508,455,133,942,304,583,236,903,222,948,"
	                   "165,808,559,332,123,348,274,797,826,204,144,"
	                   "723,168,738,177,180,919,299,881,250,404,026,"
	                   "184,124,858,368";
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

	            converter.integerGgrouping = ',';
	            converter.fractionGgrouping = '\'';

	            check() << converter(-0.0) == "0.000'000'000";
	            check() << converter(
	                -std::numeric_limits<double>::max())
	                == "-179,769,313,486,231,570,814,527,423,731,704,"
	                   "356,798,070,567,525,844,996,598,917,476,803,"
	                   "157,260,780,028,538,760,589,558,632,766,878,"
	                   "171,540,458,953,514,382,464,234,321,326,889,"
	                   "464,182,768,467,546,703,537,516,986,049,910,"
	                   "576,551,282,076,245,490,090,389,328,944,075,"
	                   "868,508,455,133,942,304,583,236,903,222,948,"
	                   "165,808,559,332,123,348,274,797,826,204,144,"
	                   "723,168,738,177,180,919,299,881,250,404,026,"
	                   "184,124,858,368.000'000'000";
            })

    ;
