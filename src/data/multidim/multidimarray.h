#ifndef MULTIDIMARRAY_H
#define MULTIDIMARRAY_H

#include <vector>
#include <functional>

#include "multidimindex.h"

namespace Vizzu
{
namespace Data
{
namespace MultiDim
{

template <typename T> class Array;

template <typename T>
class Iterator
{
public:
	Iterator(const Array<T> &parent, bool end = false);
	Iterator &operator++();

	bool operator==(const Iterator &other) const
	{ return ref == other.ref; }

	const T &operator*() { return *ref; }
	const MultiIndex &getIndex() const { return index; }

private:
	MultiIndex index;
	typename std::vector<T>::const_iterator ref;
	const Array<T> &parent;
};

template <typename T>
class Array
{
	friend class Iterator<T>;
public:
	Array() {}
	Array(const MultiIndex &sizes, const T &def = T());

	T &at(const MultiIndex &index)
	{	return values[unfoldedIndex(index)]; }

	const T &at(const MultiIndex &index) const
	{	return values[unfoldedIndex(index)]; }

	Iterator<T> begin() const { return Iterator<T>(*this, false); }
	Iterator<T> end() const { return Iterator<T>(*this, true); }

	size_t unfoldSubSliceIndex(const SubSliceIndex &) const;

	void visitSubSlice(const SubSliceIndex &subSliceIndex,
					   const std::function<void(const T&)> &visitor) const;

	void visitSubSlicesTill(const SubSliceIndex &targetSubSliceIndex,
							const std::function<void (const SubSliceIndex &)> &visitor) const;

	MultiIndex subSliceIndexMaxAt(const SubSliceIndex &subSliceIndex,
								  const MultiIndex &multiIndex) const;

	MultiIndex maxIndex() const;

	size_t lastIndexCountAt(const SubSliceIndex &subSliceIndex) const;

	bool empty() const;
	size_t unfoldedSize() const;

private:
	MultiIndex sizes;
	std::vector<T> values;

	size_t unfoldedIndex(const MultiIndex &index) const;
	void incIndex(MultiIndex &index) const;

	void visitSubSlice(const SubSliceIndex &subSliceIndex,
					   const std::function<void(const T&)> &visitor,
					   MultiIndex &multiIndex) const;

	void visitSubSlicesTill(const SubSliceIndex &targetSubSliceIndex,
							const std::function<void (const SubSliceIndex &)> &visitor,
							SubSliceIndex &subSliceIndex,
							bool whole) const;
};

}

}
}

#endif

#include "multidimarray.impl.h"
