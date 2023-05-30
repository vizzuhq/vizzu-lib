#ifndef ANIM_INTERPOLATED_H
#define ANIM_INTERPOLATED_H

#include <array>
#include <cstdint>
#include <functional>
#include <stdexcept>
#include <string>

#include "base/conv/parse.h"
#include "base/conv/tostring.h"

namespace Anim
{

template <typename Type>
class Weighted
{
public:
	Type value;
	double weight;

	Weighted() : weight(0.0) {}

	explicit Weighted(Type value) : value(std::move(value)), weight(1.0)
	{}

	explicit Weighted(Type value, double weight) :
	    value(std::move(value)),
	    weight(weight)
	{}

	template<typename ... Args>
	explicit Weighted(Args ... args) : value(args...), weight(1.0)
	{}

	bool operator==(const Weighted<Type> &other) const {
		return value == other.value
			&& weight == other.weight;
	}

	bool operator<(const Weighted<Type> &other) const {
		return value < other.value;
	}

	explicit operator bool() const {
		return weight > 0.0;
	}
};


template <typename Type>
class Interpolated
{
public:
	uint64_t count;
	std::array<Weighted<Type>,2> values;

	Interpolated() : count(1) {}
	Interpolated(const Interpolated&) = default;
	Interpolated(Interpolated&&) = default;
	Interpolated& operator=(const Interpolated&) = default;
	Interpolated& operator=(Interpolated&&) = default;

	explicit Interpolated(Type value) {
		values[0] = Weighted<Type>(std::move(value));
		count = 1;
	}

	template<typename ... Args>
	explicit Interpolated(Args ... args) {
		values[0] = Weighted<Type>(args...);
		count = 1;
	}

	explicit Interpolated(const std::string &str)
		requires(!std::is_same_v<Type, std::string>)
	{
		values[0] = Weighted<Type>(Conv::parse<Type>(str));
		count = 1;
	}

	Interpolated &operator=(Type value) {
		values[0] = Weighted<Type>(std::move(value));
		return *this;
	}

	Interpolated &operator=(Weighted<Type> weightedValue) {
		values[0] = std::move(weightedValue);
		return *this;
	}

	explicit operator bool() const {
		return count == 1 && (bool)values[0];
	}

	const Type &get() const
	{
		if (count == 1) return values[0].value;
		else throw std::logic_error("Invalid Weigthed Pair");
	}

	explicit operator std::string() const
	{
		if (count == 1) return Conv::toString(values[0].value);
		else throw std::logic_error("Invalid Weigthed Pair");
	}

	Weighted<Type> &operator*() {
		if (count == 1) return values[0];
		else throw std::logic_error("Invalid Weigthed Pair dereference");
	}

	Interpolated operator*(double val) const {
		Interpolated res(*this);
		if (res.count == 1) res.values[0].weight *= val;
		else throw std::logic_error("Cannot multiply Weigthed Pair");
		return res;
	}

	Interpolated operator+(const Interpolated &other) const
	{
		if (count != 1 || other.count != 1)
			throw std::logic_error("Cannot add Weigthed Pairs");

		Interpolated res(*this);
		if (values[0].value == other.values[0].value)
		{
			res.values[0].weight += other.values[0].weight;
		}
		else
		{
			res.values[1] = other.values[0];
			res.count = 2;
		}
		return res;
	}

	bool operator==(const Interpolated<Type> &other) const {
		if (count != other.count) return false;
		if (count == 0) return true;
		if (count == 1 && values[0] == other.values[0]) return true;
		if (count == 2
			&& values[0] == other.values[0]
			&& values[1] == other.values[1]) return true;
		return false;
	}

	bool operator==(const Type &other) const
	{
		return count == 1
			&& values[0].weight == 1.0
			&& values[0].value == other;
	}

	bool operator<(const Interpolated<Type> &other) const {
		if (count != 1 && other.count != 1)
			throw std::logic_error("cannot compare weigthed pairs");
		if (count >= other.count)
			return false;
		return values[0] < other.values[0];
	}

	void visit(const std::function<void(const Weighted<Type>&)> &branch) const
	{
		if (count >= 1 && values[0]) branch(values[0]);
		if (count >= 2 && values[1]) branch(values[1]);
	}

	template <typename T>
	T combine(const std::function<T(const Type&)> &branch) const
	{
		if (count >= 1)
		{
			auto res = branch(values[0].value) * values[0].weight;
			if (count == 2)
				res = res + branch(values[1].value) * values[1].weight;
			return res;
		}
		return T();
	}

	double factor(const Type &value) const {
		double res = 0;
		if (count >= 1 && value == values[0].value) res += values[0].weight;
		if (count >= 2 && value == values[1].value) res += values[1].weight;
		return res;
	}

	double factor() const {
		double res = 0;
		if (count >= 1) res += values[0].weight;
		if (count >= 2) res += values[1].weight;
		return res;
	}

	template <typename T = Type>
	Type calculate() const
	{
		if (this->count >= 1)
		{
			auto res = this->values[0].value * this->values[0].weight;
			if (this->count == 2)
				res = res + this->values[1].value * this->values[1].weight;
			return res;
		}
		return T();
	}

	template <typename T = Type>
	Type min() const
	{
		return
			(this->count == 1) ? this->values[0].value :
			(this->count == 2) ? std::min(this->values[0].value, this->values[1].value) :
			INFINITY;
	}

	template <typename T = Type>
	Type max() const
	{
		return
			(this->count == 1) ? this->values[0].value :
			(this->count == 2) ? std::max(this->values[0].value, this->values[1].value) :
			-INFINITY;

	}
};

typedef Interpolated<std::string> String;

}

#endif
