#ifndef IO_CONTAINERDUMPER
#define IO_CONTAINERDUMPER

#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <list>
#include <set>
#include <array>

#include "base/type/uniquelist.h"

namespace IO
{

template <class Dumper>
struct ContainerDumper
{
	struct ObjBeg{};
	struct ObjEnd{};
	struct VecBeg{};
	struct VecEnd{};
	struct Sep{};
	struct Key{ std::string str; };

	virtual ~ContainerDumper() {}

	template <class Type, size_t Size>
	Dumper &operator<<(const std::array<Type, Size> &container)
	{
		auto &stream = dynamic_cast<Dumper &>(*this);
		stream << VecBeg{};
		for (auto i = 0u; i < container.size(); i++)
		{
			if (i > 0u) stream << Sep{};
			stream << container[i];
		}
		stream << VecEnd{};
		return stream;
	}

	template <class Type, class Allocator>
	Dumper &operator<<(const std::vector<Type, Allocator> &container)
	{
		auto &stream = dynamic_cast<Dumper &>(*this);
		stream << VecBeg{};
		for (auto i = 0u; i < container.size(); i++)
		{
			if (i > 0u) stream << Sep{};
			stream << container[i];
		}
		stream << VecEnd{};
		return stream;
	}

	template <class Type, class Allocator>
	Dumper &operator<<(const std::list<Type, Allocator> &container)
	{
		auto &stream = dynamic_cast<Dumper &>(*this);
		stream << VecBeg{};
		auto i = 0u;
		for (auto &item : container)
		{
			if (i > 0u) stream << Sep{};
			stream << item;
			i++;
		}
		stream << VecEnd{};
		return stream;
	}

	template <class T>
	Dumper &operator<<(const Type::UniqueList<T> &container)
	{
		auto &stream = dynamic_cast<Dumper &>(*this);
		stream << VecBeg{};
		auto i = 0u;
		for (auto &item : container)
		{
			if (i > 0u) stream << Sep{};
			stream << item;
			i++;
		}
		stream << VecEnd{};
		return stream;
	}

	template <
	    class KeyType,
	    class ValueType,
	    class Compare,
	    class Allocator>
	Dumper &operator<<(
	    const std::map<KeyType, ValueType, Compare, Allocator>
	        &container)
	{
		auto &stream = dynamic_cast<Dumper &>(*this);
		stream << ObjBeg{};
		auto i = 0u;
		for (auto &item : container)
		{
			if (i > 0u) stream << Sep{};
			stream << Key{std::to_string(item.first)}
			       << item.second;
			i++;
		}
		stream << ObjEnd{};
		return stream;
	}
};

}

#endif

