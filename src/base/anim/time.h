#ifndef ANIM_TIME
#define ANIM_TIME

#include <chrono>
#include <string>
#include <compare>

namespace Anim
{

typedef std::chrono::steady_clock::time_point TimePoint;

class Duration : private std::chrono::duration<int64_t, std::nano>
{
public:
	typedef std::chrono::duration<int64_t, std::nano> Base;

	using Base::duration;

	Duration() : Base(0) {}
	Duration(const Base &base) : Base(base) {}
	explicit Duration(double nanosec);
	explicit Duration(const std::string &str);
	static Duration Sec(double sec);
	static Duration MSec(double millisec);
	explicit operator std::string() const;
	explicit operator double() const;
	double msec() const;
	double sec() const;

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
