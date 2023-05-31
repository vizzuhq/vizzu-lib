#ifndef DATASET_RANGE_H
#define DATASET_RANGE_H

#include "../types.h"

namespace Vizzu
{
namespace Dataset
{

class RangeIndexIterator {
friend class Range;
public:
	RangeIndexIterator();

	RangeIndexIterator& operator++();
	RangeIndexIterator operator++(int);
	bool operator==(const RangeIndexIterator& arg) const;
	bool operator!=(const RangeIndexIterator& arg) const;
	int operator*() const;

protected:
	RangeIndexIterator(const Range* owner, int head, int tail);

	static constexpr int endpos = -2;
	static constexpr int inhead = -1;
	const Range* owner;
	int headPos, tailPos;
};

class Range
{
friend class RangeIndexIterator;
public:
	Range(const ConstantSeriesPtr& series);

	ConstantSeriesPtr series() const;
	int size() const;
	ValueType typeAt(int index) const;
	Value valueAt(int index) const;
	RangeValueIterator begin() const;
	RangeValueIterator end() const;
	RangeIndexIterator indices_begin(RangeValueIterator iter) const;
	RangeIndexIterator indices_end(RangeValueIterator iter) const;

protected:
	struct IndexItem {
		int seriesIndex;
		int nextItem;
	};

	using index_vector = std::vector<IndexItem>;

	ConstantSeriesPtr valueSet;
	index_vector headValues;
	index_vector tailValues;

	void generate();
};

}
}

#endif // DATASET_RANGE_H
