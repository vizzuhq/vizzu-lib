#ifndef MULTIDIMINDEX_H
#define MULTIDIMINDEX_H

#include <ranges>
#include <vector>

#include "base/text/smartstring.h"
#include "base/type/uniquetype.h"

namespace Vizzu::Data::MultiDim
{

using DimIndex = Type::UniqueType<uint64_t, struct DimIndexTypeId>;

using Index = Type::UniqueType<uint64_t, struct IndexTypeId>;

using MultiIndex = std::vector<Index>;

struct SliceIndex
{
	DimIndex dimIndex;
	Index index;

	bool operator==(const SliceIndex &other) const
	{
		return dimIndex == other.dimIndex && index == other.index;
	}

	bool operator<(const SliceIndex &other) const
	{
		return dimIndex < other.dimIndex
		    || (dimIndex == other.dimIndex && index < other.index);
	}
};

class SubSliceIndex : public std::vector<SliceIndex>
{
public:
	SubSliceIndex() = default;

	explicit SubSliceIndex(const MultiDim::MultiIndex &multiIndex)
	{
		for (auto i = 0U; i < multiIndex.size(); i++)
			this->push_back({DimIndex(i), multiIndex[i]});
	}

	SubSliceIndex(const MultiDim::MultiIndex &multiIndex,
	    const std::vector<DimIndex> &dimensions)
	{
		for (auto dimIndex : dimensions)
			this->push_back({dimIndex, multiIndex[dimIndex]});
	}

	[[nodiscard]] std::vector<DimIndex> dimensions() const
	{
		std::vector<DimIndex> res;
		for (const auto &sliceIndex : *this)
			res.push_back(sliceIndex.dimIndex);
		return res;
	}

	bool getIndexIfPresent(DimIndex dimIndex, Index &index) const
	{
		for (const auto &sliceIndex : *this)
			if (sliceIndex.dimIndex == dimIndex) {
				index = sliceIndex.index;
				return true;
			}
		return false;
	}

	[[nodiscard]] bool contains(const MultiIndex &multiIndex) const
	{
		return std::all_of(begin(), end(),
		    [&](const auto &sliceIndex)
		    {
			    return multiIndex[sliceIndex.dimIndex]
			        == sliceIndex.index;
		    });
	}

	[[nodiscard]] MultiIndex getProjectionOf(
	    const MultiIndex &multiIndex) const
	{
		MultiIndex res = multiIndex;
		for (const auto &sliceIndex : *this)
			res[sliceIndex.dimIndex] = sliceIndex.index;
		return res;
	}

	[[nodiscard]] bool hardEqual(const SubSliceIndex &other) const
	{
		if (size() != other.size()) return false;
		for (auto i = 0U; i < size(); i++)
			if ((*this)[i] != other[i]) return false;
		return true;
	}
};
static_assert(
    sizeof(SubSliceIndex) == sizeof(std::vector<SliceIndex>));

}

#endif
