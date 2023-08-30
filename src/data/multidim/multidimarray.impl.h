#ifndef MULTIDIMARRAY_IMPL_H
#define MULTIDIMARRAY_IMPL_H

#include <stdexcept>
#include <utility>

#include "multidimarray.h"

namespace Vizzu::Data::MultiDim
{

template <typename T>
Array<T>::Array(MultiIndex sizes, const T &def) :
    sizes(std::move(sizes))
{
	values.resize(unfoldedSize());
	for (auto &value : values) value = def;
}

template <typename T> size_t Array<T>::unfoldedSize() const
{
	size_t unfoldedSize = 1U;
	for (auto size : sizes) unfoldedSize *= size;
	return unfoldedSize;
}

template <typename T>
size_t Array<T>::unfoldedIndex(const MultiIndex &index) const
{
	if (index.size() != sizes.size())
		throw std::logic_error(
		    "internal error: multi dimensional array size missmatch");

	if (index.empty()) return 0U;

	size_t unfoldedIndex = 0U;
	for (auto i = 0U; i < sizes.size(); i++) {
		auto size = sizes[i];
		auto idx = index[i];
		unfoldedIndex *= size;
		unfoldedIndex += idx;

		if (idx >= size)
			throw std::logic_error(
			    "internal error: multi dimensional array index out "
			    "of range");
	}
	return unfoldedIndex;
}

template <typename T>
size_t Array<T>::unfoldSubSliceIndex(
    const SubSliceIndex &subSliceIndex) const
{
	size_t unfoldedIndex = 0U;

	for (const auto &sliceIndex : subSliceIndex) {
		auto size = sizes[sliceIndex.dimIndex];
		auto idx = sliceIndex.index;
		unfoldedIndex *= size;
		unfoldedIndex += idx;
	}
	return unfoldedIndex;
}

template <typename T>
void Array<T>::visitSubSlice(const SubSliceIndex &subSliceIndex,
    const std::function<void(const T &)> &visitor) const
{
	MultiIndex multiIndex;
	multiIndex.reserve(sizes.size());
	this->visitSubSlice(subSliceIndex, visitor, multiIndex);
}

template <typename T>
void Array<T>::visitSubSlicesTill(
    const SubSliceIndex &targetSubSliceIndex,
    const std::function<void(const SubSliceIndex &)> &visitor) const
{
	SubSliceIndex subSliceIndex;
	subSliceIndex.reserve(targetSubSliceIndex.size());
	this->visitSubSlicesTill(targetSubSliceIndex,
	    visitor,
	    subSliceIndex,
	    false);
}

template <typename T>
void Array<T>::visitSubSlice(const SubSliceIndex &subSliceIndex,
    const std::function<void(const T &)> &visitor,
    MultiIndex &multiIndex) const
{
	if (multiIndex.size() == sizes.size())
		visitor(at(multiIndex));
	else {
		multiIndex.emplace_back(0);
		auto dim = DimIndex(multiIndex.size() - 1);
		Index index;
		if (subSliceIndex.getIndexIfPresent(dim, index)) {
			multiIndex[dim] = index;
			visitSubSlice(subSliceIndex, visitor, multiIndex);
		}
		else
			for (auto i = 0U; i < sizes[dim]; i++) {
				multiIndex[dim] = Index(i);
				visitSubSlice(subSliceIndex, visitor, multiIndex);
			}
		multiIndex.resize(dim);
	}
}

template <typename T>
void Array<T>::visitSubSlicesTill(
    const SubSliceIndex &targetSubSliceIndex,
    const std::function<void(const SubSliceIndex &)> &visitor,
    SubSliceIndex &subSliceIndex,
    bool whole) const
{
	if (subSliceIndex.size() == targetSubSliceIndex.size()) {
		visitor(subSliceIndex);
	}
	else {
		auto level = subSliceIndex.size();
		auto dimIndex = targetSubSliceIndex[level].dimIndex;
		subSliceIndex.push_back({dimIndex, Index(0)});

		auto maxIndex = whole ? sizes[dimIndex] - 1
		                      : targetSubSliceIndex[level].index;

		for (auto i = 0U; i <= maxIndex; i++) {
			subSliceIndex[level].index = Index(i);
			visitSubSlicesTill(targetSubSliceIndex,
			    visitor,
			    subSliceIndex,
			    i != targetSubSliceIndex[level].index);
		}
		subSliceIndex.pop_back();
	}
}

template <typename T>
MultiIndex Array<T>::subSliceIndexMaxAt(
    const SubSliceIndex &subSliceIndex,
    const MultiIndex &multiIndex) const
{
	MultiIndex res = multiIndex;
	for (const auto &sliceIndex : subSliceIndex)
		res[sliceIndex.dimIndex] =
		    Index{sizes[sliceIndex.dimIndex] - 1};
	return res;
}

template <typename T>
size_t Array<T>::lastIndexCountAt(
    const SubSliceIndex &subSliceIndex) const
{
	auto count = 0;
	for (const auto &sliceIndex : subSliceIndex)
		if (sliceIndex.index == sizes[sliceIndex.dimIndex] - 1)
			count++;
	return count;
}

template <typename T> MultiIndex Array<T>::maxIndex() const
{
	MultiIndex res = sizes;
	for (auto &index : res) index = Index(index - 1);
	return res;
}

template <typename T> bool Array<T>::empty() const
{
	return values.empty();
}

template <typename T> void Array<T>::incIndex(MultiIndex &index) const
{
	int dim = static_cast<int>(index.size()) - 1;
	bool overflow = true;
	while (overflow && dim >= 0) {
		index[dim]++;
		if (index[dim] >= sizes[dim])
			index[dim] = Index(0);
		else
			overflow = false;
		dim--;
	}
}

template <typename T>
Iterator<T>::Iterator(const Array<T> &parent, bool end) :
    ref(end ? parent.values.end() : parent.values.begin()),
    parent(parent)
{
	index = MultiIndex(parent.sizes.size(), Index(0));
}

template <typename T> Iterator<T> &Iterator<T>::operator++()
{
	ref++;
	parent.incIndex(index);
	return *this;
}

}

#endif
