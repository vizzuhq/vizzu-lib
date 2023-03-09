#ifndef DATASET_SERIES_H
#define DATASET_SERIES_H

#include "types.h"
#include "interfaces.h"

namespace Vizzu
{
namespace Dataset
{

class Series : public AbstractSeries
{
public:
	Series();

	ValueType type() const override;
	int size() const override;
	ValueType typeAt(int index) const override;
	Value valueAt(int index) const override;
	ValueIterator begin() const override;
	ValueIterator end() const override;
	RangePtr range() const override;

protected:
	SeriesId id;
	ValueType vtype;
	SeriesIndexPtr index;
	SeriesPtr data;
	SeriesInfo info;
};

class SeriesIndex
{
public:
	SeriesIndex();

	int size() const;
	void resize(int newSize);
	int &at(int arg);
	ValueIterator begin() const;
	ValueIterator end() const;

protected:
	IndexVector forwardIndices;
	IndexVector backwardIndices;
};

class SeriesContainer
{
friend class Dataset;
public:
	static const SeriesId nullId = 0;

	int size() const;
	void clear();
	SeriesIterator begin() const;
	SeriesIterator end() const;
	SeriesPtr getSeries(SeriesId id) const;
	SeriesPtr getSeries(const char *name) const;

protected:
	static uint32_t nextSeriesId;
	Dataset &dataset;
	SeriesMap series;

	SeriesContainer(Dataset &dataset);
	SeriesId insert(SeriesPtr ptr);
};

}
}

#endif // DATASET_SERIES_H
