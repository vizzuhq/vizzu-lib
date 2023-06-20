#ifndef UTIL_TEMPLATES
#define UTIL_TEMPLATES

#include <map>
#include <stdexcept>
#include <type_traits>

namespace Util
{

template <class FriendClass> struct Friend
{
	template <class Base> class Protected : protected Base
	{
		friend FriendClass;
	};
};

template <class T> class ReadWrite
{
public:
	typedef T UnderlyingType;
	const T &get() const { return value; }
	T &ref() { return value; }
	template <class TT> bool set(const TT &val)
	{
		if (value != val) {
			value = val;
			return true;
		}
		else
			return false;
	}

private:
	T value;
};

template <class T, class R = void> struct HasUnderlyingTypeHelper
{
	typedef R type;
};

template <class T, class Enable = void>
struct HasUnderlyingType : std::false_type
{};

template <class T>
struct HasUnderlyingType<T,
    typename HasUnderlyingTypeHelper<
        typename T::UnderlyingType>::type> : std::true_type
{};

template <typename T> struct ReversionWrapper
{
	const T &iterable;

	auto begin() const { return iterable.rbegin(); }
	auto end() const { return iterable.rend(); }
};

template <typename T> ReversionWrapper<T> Reverse(const T &iterable)
{
	return {iterable};
}
}

#endif
