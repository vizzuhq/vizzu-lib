#ifndef MULTIDIMINDEX_H
#define MULTIDIMINDEX_H

#include <vector>

#include "base/text/smartstring.h"
#include "base/type/uniquetype.h"

namespace Vizzu
{
namespace Data
{
namespace MultiDim
{

struct DimIndexTypeId
{};
typedef Type::UniqueType<uint64_t, DimIndexTypeId> DimIndex;

struct IndexTypeId
{};
typedef Type::UniqueType<uint64_t, IndexTypeId> Index;

typedef std::vector<Index> MultiIndex;

static inline std::string to_string(const MultiIndex &multiIndex)
{
	typedef Text::SmartString S;
	return "[ "
	     + S::join<std::vector, std::string>(
	         S::map(multiIndex,
	             [](const Index &index)
	             {
		             return std::to_string(static_cast<size_t>(index));
	             }),
	         std::string(", "))
	     + " ]";
}

struct SliceIndex
{
	DimIndex dimIndex;
	Index index;

	operator std::string() const
	{
		return "dim: " + std::to_string(static_cast<size_t>(dimIndex))
		     + ","
		       "idx: "
		     + std::to_string(static_cast<size_t>(index));
	}

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
	SubSliceIndex() {}

	SubSliceIndex(const MultiDim::MultiIndex &multiIndex)
	{
		for (auto i = 0u; i < multiIndex.size(); i++)
			this->push_back({DimIndex(i), multiIndex[i]});
	}

	SubSliceIndex(const MultiDim::MultiIndex &multiIndex,
	    const std::vector<DimIndex> &dimensions)
	{
		for (auto dimIndex : dimensions)
			this->push_back({dimIndex, multiIndex[dimIndex]});
	}

	std::vector<DimIndex> dimensions() const
	{
		std::vector<DimIndex> res;
		for (auto &sliceIndex : *this)
			res.push_back(sliceIndex.dimIndex);
		return res;
	}

	bool getIndexIfPresent(DimIndex dimIndex, Index &index) const
	{
		for (auto &sliceIndex : *this)
			if (sliceIndex.dimIndex == dimIndex) {
				index = sliceIndex.index;
				return true;
			}
		return false;
	}

	bool contains(const MultiIndex &multiIndex) const
	{
		for (auto &sliceIndex : *this)
			if (multiIndex[sliceIndex.dimIndex] != sliceIndex.index)
				return false;
		return true;
	}

	MultiIndex getProjectionOf(const MultiIndex &multiIndex) const
	{
		MultiIndex res = multiIndex;
		for (auto &sliceIndex : *this)
			res[sliceIndex.dimIndex] = sliceIndex.index;
		return res;
	}

	operator std::string() const
	{
		typedef Text::SmartString S;
		return "[ "
		     + S::join<std::vector, SliceIndex>(*this,
		         std::string(", "))
		     + " ]";
	}

	bool hardEqual(const SubSliceIndex &other) const
	{
		if (size() != other.size()) return false;
		for (auto i = 0u; i < size(); i++)
			if ((*this)[i] != other[i]) return false;
		return true;
	}
};
static_assert(sizeof(SubSliceIndex)
                  == sizeof(std::vector<SliceIndex>),
    "");

}
}
}

#endif
