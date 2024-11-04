#ifndef ANIM_INTERPOLATED_H
#define ANIM_INTERPOLATED_H

#include <array>
#include <cstdint>
#include <functional>
#include <stdexcept>
#include <string>

#include "base/conv/parse.h"
#include "base/conv/tostring.h"
#include "base/math/floating.h"
#include "base/math/interpolation.h"

namespace Anim
{

template <typename Type> class Weighted
{
public:
	Type value{};
	double weight{};

	Weighted() noexcept(
	    std::is_nothrow_default_constructible_v<Type>) = default;

	explicit Weighted(Type value, double weight = 1.0) :
	    value(std::move(value)),
	    weight(weight)
	{}

	bool operator==(const Weighted<Type> &other) const
	{
		return value == other.value && weight == other.weight;
	}

	bool operator<(const Weighted<Type> &other) const
	{
		using Less = std::conditional_t<std::floating_point<Type>,
		    decltype(Math::Floating::less),
		    std::less<Type>>;
		return Less{}(value, other.value);
	}

	[[nodiscard]] bool hasValue() const { return weight > 0.0; }
};

enum class InterpolateIndex : bool { first = false, second = true };

using enum InterpolateIndex;

inline InterpolateIndex operator||(const InterpolateIndex &lhs,
    const InterpolateIndex &rhs)
{
	return static_cast<InterpolateIndex>(
	    static_cast<bool>(lhs) || static_cast<bool>(rhs));
}

template <typename Type> class Interpolated
{
public:
	std::array<Weighted<Type>, 2> values;
	bool has_second{};

	Interpolated() noexcept(
	    std::is_nothrow_default_constructible_v<Type>) = default;
	Interpolated(const Interpolated &) noexcept(
	    std::is_nothrow_copy_constructible_v<Type>) = default;
	Interpolated(Interpolated &&) noexcept(
	    std::is_nothrow_move_constructible_v<Type>) = default;
	Interpolated &operator=(const Interpolated &) noexcept(
	    std::is_nothrow_copy_assignable_v<Type>) = default;
	Interpolated &operator=(Interpolated &&) noexcept(
	    std::is_nothrow_move_assignable_v<Type>) = default;

	explicit Interpolated(Type value)
	{
		values[0] = Weighted<Type>(std::move(value));
	}

	explicit Interpolated(const std::string &str)
	    requires(!std::is_same_v<Type, std::string>)
	{
		values[0] = Weighted<Type>(Conv::parse<Type>(str));
	}

	Interpolated &operator=(Type value)
	{
		values[0] = Weighted<Type>(std::move(value));
		return *this;
	}

	Interpolated &operator=(Weighted<Type> weightedValue)
	{
		values[0] = std::move(weightedValue);
		return *this;
	}

	[[nodiscard]] bool hasOneValue() const
	{
		return !has_second && values[0].hasValue();
	}

	[[nodiscard]] bool interpolates() const { return has_second; }

	[[nodiscard]] InterpolateIndex maxIndex() const
	{
		return static_cast<InterpolateIndex>(has_second);
	}

	[[nodiscard]] Interpolated shifted() const
	{
		if (has_second)
			throw std::logic_error("Cannot move Weigthed Value");

		Interpolated res;
		res.values[1] = values[0];
		res.has_second = true;
		return res;
	}

	[[nodiscard]] const Type &get() const
	{
		if (!has_second) return values[0].value;

		throw std::logic_error("Invalid Weigthed Pair");
	}

	[[nodiscard]] const Weighted<Type> &get_or_first(
	    InterpolateIndex index) const
	{
		return values[has_second && static_cast<bool>(index)];
	}

	[[nodiscard]] std::optional<InterpolateIndex> get_index(
	    const Type &type) const
	{
		if (values[0].value == type) return first;
		if (has_second && values[1].value == type) return second;
		return {};
	}

	template <class T>
	// NOLINTNEXTLINE(cppcoreguidelines-missing-std-forward)
	[[nodiscard]] auto get(T &&) const = delete;

	[[nodiscard]] auto toString() const
	{
		if (!has_second) return Conv::toString(values[0].value);

		throw std::logic_error("Invalid Weigthed Pair");
	}

	Weighted<Type> &operator*() { return *operator->(); }

	Weighted<Type> *operator->()
	{
		if (!has_second) return values.data();

		throw std::logic_error("Invalid Weigthed Pair dereference");
	}

	bool operator==(const Interpolated<Type> &other) const
	{
		return has_second == other.has_second
		    && values[0] == other.values[0]
		    && (!has_second || values[1] == other.values[1]);
	}

	bool operator==(const Type &other) const
	{
		return !has_second && values[0].weight == 1.0
		    && values[0].value == other;
	}

	bool operator<(const Interpolated<Type> &other) const
	{
		if (has_second && other.has_second)
			throw std::logic_error("cannot compare weigthed pairs");
		if (!other.has_second) return false;
		return values[0] < other.values[0];
	}

	template <class T> void visit(T &&branch) const
	{
		if (values[0].hasValue()) branch(first, values[0]);
		if (has_second && values[1].hasValue())
			std::forward<T>(branch)(second, values[1]);
	}

	template <class U = void,
	    class Fun,
	    class T = std::conditional_t<std::is_void_v<U>,
	        std::invoke_result_t<Fun, Type>,
	        U>>
	T combine(Fun &&branch) const
	{
		auto res = static_cast<T>(branch(values[0].value))
		         * values[0].weight;
		if (has_second)
			res = res
			    + static_cast<T>(
			          std::forward<Fun>(branch)(values[1].value))
			          * values[1].weight;
		return static_cast<T>(res);
	}

	[[nodiscard]] bool contains(const Type &value) const
	{
		if (value == values[0].value) return true;
		if (has_second && value == values[1].value) return true;
		return false;
	}

	template <class T = double, class U>
	[[nodiscard]] T factor(const U &value) const
	{
		double res{};
		if (values[0].value == value) res += values[0].weight;
		if (has_second && values[1].value == value)
			res += values[1].weight;
		return T{res};
	}

	template <typename T = Type> [[nodiscard]] T calculate() const
	{
		auto res = static_cast<T>(this->values[0].value)
		         * this->values[0].weight;
		if (has_second)
			res = res
			    + static_cast<T>(this->values[1].value)
			          * this->values[1].weight;
		return res;
	}

	template <typename T = Type> [[nodiscard]] T min() const
	{
		using Less = std::conditional_t<std::floating_point<T>,
		    decltype(Math::Floating::less),
		    std::less<T>>;
		return !has_second ? this->values[0].value
		                   : std::min(this->values[0].value,
		                       this->values[1].value,
		                       Less{});
	}

	template <typename T = Type> [[nodiscard]] T max() const
	{
		using Less = std::conditional_t<std::floating_point<T>,
		    decltype(Math::Floating::less),
		    std::less<T>>;
		return !has_second ? this->values[0].value
		                   : std::max(this->values[0].value,
		                       this->values[1].value,
		                       Less{});
	}
};

template <typename Type>
Interpolated<Type> interpolate(const Interpolated<Type> &op0,
    const Interpolated<Type> &op1,
    double factor)
{
	if (factor <= 0.0) return op0;
	if (factor > 1.0) factor = 1.0;

	if (op0.has_second || op1.has_second)
		throw std::logic_error("Cannot interpolate Weigthed Pairs");

	Interpolated<Type> res;
	if (op0.values[0].value == op1.values[0].value) {
		res.values[0].value = op0.values[0].value;
		res.values[0].weight = Math::interpolate(op0.values[0].weight,
		    op1.values[0].weight,
		    factor);
	}
	else {
		res.values[0].value = op0.values[0].value;
		res.values[0].weight = op0.values[0].weight * (1.0 - factor);
		res.values[1].value = op1.values[0].value;
		res.values[1].weight = op1.values[0].weight * factor;
		res.has_second = true;
	}
	return res;
}

using String = Interpolated<std::string>;

}

#endif
