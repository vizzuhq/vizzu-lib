#ifndef UTIL_TEMPLATES
#define UTIL_TEMPLATES

#include <set>
#include <stdexcept>
#include <type_traits>

namespace Util
{

template <class FriendClass>
struct Friend
{
	template <class Base>
	class Protected : protected Base { friend FriendClass; };
};

template <class Parent, class T>
class ReadOnly {
	friend Parent;
public:
	typedef T UnderlyingType;
	const T &operator()() const { return data; }
private:
	ReadOnly() = default;
	ReadOnly(const ReadOnly<Parent, T> &) = default;
	ReadOnly(const T &arg) : data(arg) {}
	T &operator *() { return data; }
	T *operator->() { return &data; }
	void operator=(const T &arg) { data = arg; }
	T data;
};

template <class Derived>
struct AddReadOnly
{
	template<class T> using ReadOnly = Util::ReadOnly<Derived, T>;
};

template <class T>
class ReadWrite {
public:
	typedef T UnderlyingType;
	const T &get() const { return value; }
	T &ref() { return value; }
	template <class TT>
	bool set(const TT &val) {
		if (value != val) {
			value = val;
			return true;
		} else return false;
	}
private:
	T value;
};

template<class T, class R = void>
struct HasUnderlyingTypeHelper { typedef R type; };

template<class T, class Enable = void>
struct HasUnderlyingType : std::false_type{};

template<class T>
struct HasUnderlyingType<T, typename HasUnderlyingTypeHelper<typename T::UnderlyingType>::type> : std::true_type {};

template <typename T>
struct ReversionWrapper
{
	const T& iterable;

	auto begin() const { return iterable.rbegin(); }
	auto end() const { return iterable.rend(); }
};

template <typename T>
ReversionWrapper<T> Reverse(const T& iterable) { return { iterable }; }
}

template<typename T>
std::set<T> operator&(const std::set<T>&, const std::set<T>&) {
	return std::set<T>{};
}

template<typename T>
std::set<T> operator^(const std::set<T>&, const std::set<T>&) {
	return std::set<T>{};
}

#endif
