
#include "numtostr.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <limits>
#include <stdexcept>
#include <string>
#include <string_view>

namespace Conv
{

std::string NumberToString::convert(double number)
{
	auto *begin = std::data(buffer);

	const auto precision = std::pow(10, fractionDigitCount);
	const auto roundPoint = 0.5 / precision;
	if (const auto remainder = std::fmod(number, 1 / precision);
	    remainder < roundPoint
	    && roundPoint - remainder
	           < std::numeric_limits<double>::epsilon())
		if (const auto next = std::nextafter(number,
		        std::numeric_limits<double>::max()),
		    other_remainder = std::fmod(next, 1 / precision);
		    other_remainder > roundPoint
		    && other_remainder - roundPoint >= roundPoint - remainder)
			number = next;

	std::string_view number_view(begin,
	    std::snprintf(begin,
	        MAX_BUFFER_SIZE,
	        ("%." + std::to_string(fractionDigitCount) + "f").c_str(),
	        number));

	auto decimalPoint = std::min({number_view.find(','),
	    number_view.find('.'),
	    number_view.size()});
	if (decimalPoint != number_view.size()) {
		if (!fillFractionWithZero) {
			number_view = number_view.substr(0,
			    number_view.find_last_not_of('0') + 1);
		}
		if (number_view.ends_with(',')
		    || number_view.ends_with('.')) {
			number_view.remove_suffix(1);
		}
	}

	if (number_view.starts_with('-')
	    && number_view.find_last_not_of("0,.") == 0) {
		number_view.remove_prefix(1);
		++begin;
		--decimalPoint;
	}

	if (decimalPoint != number_view.size())
		begin[decimalPoint] = decimalPointChar;

	auto int_groups_count =
	    static_cast<int>(integerGgrouping != '\0')
	    * (decimalPoint - 1
	        - static_cast<int>(number_view.starts_with('-')))
	    / 3;

	auto frac_groups_count =
	    static_cast<int>(fractionGgrouping != '\0')
	    * (static_cast<intptr_t>(number_view.size() - decimalPoint)
	        - 2)
	    / 3;
	const bool move_fractions = frac_groups_count > 0;

	const auto full_size =
	    number_view.size() + int_groups_count + frac_groups_count;

	if (full_size > MAX_BUFFER_SIZE)
		throw std::runtime_error(
		    "NumToStr serialize failed - buffer is small");

	auto *last_place = begin + full_size;

	for (; frac_groups_count > 0; --frac_groups_count) {
		last_place = std::ranges::copy_backward(
		    number_view.substr(decimalPoint + 1
		                           + frac_groups_count * 3,
		        3),
		    last_place)
		                 .out;
		*--last_place = fractionGgrouping;
	}

	if (int_groups_count > 0) {
		last_place = std::ranges::copy_backward(
		    number_view.substr(decimalPoint,
		        move_fractions ? 4 : std::string_view::npos),
		    last_place)
		                 .out;

		for (size_t i{1}; i <= int_groups_count; ++i) {
			last_place = std::ranges::copy_backward(
			    number_view.substr(decimalPoint - i * 3, 3),
			    last_place)
			                 .out;
			*--last_place = integerGgrouping;
		}
	}
	return {begin, full_size};
}

std::string NumberToString::operator()(double number)
{
	return convert(number);
}

}