#ifndef BASE_ANIM_TIME_H
#define BASE_ANIM_TIME_H

#include <chrono>
#include <compare>
#include <string>

namespace Anim
{

using TimePoint = std::chrono::steady_clock::time_point;

class Duration : private std::chrono::duration<int64_t, std::nano>
{
public:
	using Base = std::chrono::duration<int64_t, std::nano>;

	using Base::duration;

	Duration() : Base(0) {}
	Duration(const Base &base) : Base(base) {}
	explicit Duration(double nanosec);
	explicit Duration(const std::string &str);
	static Duration Sec(double sec);
	static Duration MSec(double millisec);
	explicit operator std::string() const;
	explicit operator double() const;
	[[nodiscard]] double msec() const;
	[[nodiscard]] double sec() const;

	Duration operator-(const Duration &other) const;
	Duration operator+(const Duration &other) const;
	Duration &operator-=(const Duration &other);
	Duration &operator+=(const Duration &other);
	double operator/(const Duration &other) const;
	Duration operator*(double other) const;

	bool operator==(const Duration &other) const;
	bool operator<=(const Duration &other) const;
	bool operator<(const Duration &other) const;
	bool operator>=(const Duration &other) const;
	bool operator>(const Duration &other) const;
};

}

#endif
