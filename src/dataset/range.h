#ifndef DATASET_RANGE_H
#define DATASET_RANGE_H

#include "types.h"

namespace Vizzu
{
namespace Dataset
{

class Range
{
friend class index_iter;
public:
	using value_iter = IndexBasedIterator<const Range*>;
	
	class index_iter {
	friend class Range;
	public:
		index_iter();

		index_iter& operator++();
		index_iter operator++(int);
		bool operator==(const index_iter& arg) const;
		int operator*() const;

	protected:
		index_iter(const Range* owner, int head, int tail);

		static constexpr int endpos = -2;
		static constexpr int inhead = -1;
		const Range* owner;
		int headPos, tailPos;
	};

public:
	Range(const ConstSeriesPtr& series);

	ConstSeriesPtr series() const;
	int size() const;
	ValueType typeAt(int index) const;
	Value valueAt(int index) const;
	value_iter begin() const;
	value_iter end() const;
	index_iter indices_begin(value_iter iter) const;
	index_iter indices_end(value_iter iter) const;

protected:
	struct IndexItem {
		int seriesIndex;
		int nextItem;
	};

	using index_vector = std::vector<IndexItem>;

	ConstSeriesPtr underliyngSeries;
	index_vector headValues;
	index_vector tailValues;

	void generate();
};

}
}

#endif // DATASET_RANGE_H
