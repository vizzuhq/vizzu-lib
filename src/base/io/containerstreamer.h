#ifndef IO_CONTAINERSTREAMER
#define IO_CONTAINERSTREAMER

#include <string>
#include <cstdint>
#include <map>
#include <unordered_map>
#include <vector>
#include <list>
#include <set>
#include <array>

namespace IO
{

template<class BinaryFormatter, class Type, size_t Size>
BinaryFormatter& operator<<(BinaryFormatter &stream, const std::array<Type, Size> &container)
{
	for (auto &item : container) stream << item;
	return stream;
}

template<class BinaryFormatter, class Type, size_t Size>
BinaryFormatter& operator>>(BinaryFormatter &stream, std::array<Type, Size> &container)
{
	for (auto i = 0u; i < Size; i++) {
		Type value;
		stream >> value;
		container[i] = value;
	}
	return stream;
}

template<class BinaryFormatter, class Type, class Allocator>
BinaryFormatter& operator<<(BinaryFormatter &stream, const std::vector<Type, Allocator> &container)
{
	stream << (uint64_t)container.size();
	for (auto &item : container) stream << item;
	return stream;
}

template<class BinaryFormatter, class Type, class Allocator>
BinaryFormatter& operator>>(BinaryFormatter &stream, std::vector<Type, Allocator> &container)
{
	typedef std::vector<Type, Allocator> Cont;
	uint64_t size;
	stream >> size;
	container.reserve(size);
	for (auto i = 0u; i < size; i++) {
		typename Cont::value_type value;
		stream >> value;
		container.push_back(value);
	}
	return stream;
}

template<class BinaryFormatter, class Type, class Allocator>
BinaryFormatter& operator<<(BinaryFormatter &stream, const std::list<Type, Allocator> &container)
{
	stream << (uint64_t)container.size();
	for (auto &item : container) stream << item;
	return stream;
}

template<class BinaryFormatter, class Type, class Allocator>
BinaryFormatter& operator>>(BinaryFormatter &stream, std::list<Type, Allocator> &container)
{
	typedef std::list<Type, Allocator> Cont;
	uint64_t size;
	stream >> size;
	for (auto i = 0u; i < size; i++) {
		typename Cont::value_type value;
		stream >> value;
		container.push_back(value);
	}
	return stream;
}

template<class BinaryFormatter, class KeyType, class ValueType, class Compare, class Allocator>
BinaryFormatter& operator<<(BinaryFormatter &stream, const std::map<KeyType, ValueType, Compare, Allocator> &container)
{
	stream << (uint64_t)container.size();
	for (auto &item : container) stream << item.first << item.second;
	return stream;
}

template<class BinaryFormatter, class KeyType, class ValueType, class Compare, class Allocator>
BinaryFormatter& operator>>(BinaryFormatter &stream, std::map<KeyType, ValueType, Compare, Allocator> &container)
{
	typedef std::map<KeyType, ValueType, Compare, Allocator> Cont;
	uint64_t size;
	stream >> size;
	for (auto i = 0u; i < size; i++) {
		typename Cont::key_type key;
		typename Cont::mapped_type value;
		stream >> key;
		stream >> value;
		container[key] = value;
	}
	return stream;
}

}

#endif

