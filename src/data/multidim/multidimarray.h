#ifndef MULTIDIMARRAY_H
#define MULTIDIMARRAY_H

#include <functional>
#include <vector>

#include "multidimindex.h"

namespace Vizzu::Data::MultiDim
{

template <typename T> class Array;

template <typename T> class Iterator
{
public:
	explicit Iterator(const Array<T> &parent, bool end = false);
	Iterator &operator++();

	bool operator==(const Iterator &other) const
	{
		return ref == other.ref;
	}

	const T &operator*() { return *ref; }
	[[nodiscard]] const MultiIndex &getIndex() const { return index; }

private:
	MultiIndex index;
	typename std::vector<T>::const_iterator ref;
	const Array<T> &parent;
};

template <typename T> class Array
{
	friend class Iterator<T>;

public:
	Array() = default;
	explicit Array(MultiIndex sizes, const T &def = T());

	T &at(const MultiIndex &index)
	{
		return values[unfoldedIndex(index)];
	}

	[[nodiscard]] const T &at(const MultiIndex &index) const
	{
		return values[unfoldedIndex(index)];
	}

	[[nodiscard]] Iterator<T> begin() const
	{
		return Iterator<T>(*this, false);
	}
	[[nodiscard]] Iterator<T> end() const
	{
		return Iterator<T>(*this, true);
	}

	[[nodiscard]] size_t unfoldSubSliceIndex(
	    const SubSliceIndex &) const;

	void visitSubSlice(const SubSliceIndex &subSliceIndex,
	    const std::function<void(const T &)> &visitor) const;

	void visitSubSlicesTill(const SubSliceIndex &targetSubSliceIndex,
	    const std::function<void(const SubSliceIndex &)> &visitor)
	    const;

	[[nodiscard]] MultiIndex subSliceIndexMaxAt(
	    const SubSliceIndex &subSliceIndex,
	    const MultiIndex &multiIndex) const;

	[[nodiscard]] MultiIndex maxIndex() const;

	[[nodiscard]] size_t lastIndexCountAt(
	    const SubSliceIndex &subSliceIndex) const;

	[[nodiscard]] bool empty() const;
	[[nodiscard]] size_t unfoldedSize() const;
	[[nodiscard]] size_t unfoldedIndex(const MultiIndex &index) const;

private:
	MultiIndex sizes;
	std::vector<T> values;

	void incIndex(MultiIndex &index) const;

	void visitSubSlice(const SubSliceIndex &subSliceIndex,
	    const std::function<void(const T &)> &visitor,
	    MultiIndex &multiIndex) const;

	void visitSubSlicesTill(const SubSliceIndex &targetSubSliceIndex,
	    const std::function<void(const SubSliceIndex &)> &visitor,
	    SubSliceIndex &subSliceIndex,
	    bool whole) const;
};

}

#endif

#include "multidimarray.impl.h"
