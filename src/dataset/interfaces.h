#ifndef DATAFRAME_INTERFACES_H
#define DATAFRAME_INTERFACES_H

#include "types.h"

namespace Vizzu
{
namespace Dataset
{

class AbstractConstantSeries
{
public:
	virtual Dataset& owner() const = 0;
	virtual int size() const = 0;
	virtual SeriesId id() const = 0;
	virtual const char* name() const = 0;
	virtual ValueType type() const = 0;
	virtual ValueType typeAt(int index) const = 0;
	virtual Value valueAt(int index) const = 0;
	virtual ValueIterator begin() const = 0;
	virtual ValueIterator end() const = 0;
	
	virtual ~AbstractConstantSeries() {}
};

class AbstractVolatileSeries :
	public AbstractConstantSeries
{
public:
    virtual void extend(int size) = 0;
    virtual void insert(double value, int position = nullpos) = 0;
    virtual void insert(const char* value, int position = nullpos) = 0;
    virtual void insert(ValueType vt, const Value& value, int position = nullpos) = 0;
    virtual void modify(double value, int position) = 0;
    virtual void modify(const char* value, int position) = 0;
    virtual void modify(ValueType vt, const Value& value, int position) = 0;
    virtual void erase(int index) = 0;

	virtual ~AbstractVolatileSeries() {}
};

class AbstractTableGenerator {
public:
    virtual ~AbstractTableGenerator() {}
};

class AbstractSeriesGenerator {
public:
    virtual ~AbstractSeriesGenerator() {}
};

class AbstractSeriesAggregator {
public:
    virtual ~AbstractSeriesAggregator() {}
};

class AbstractFilter {
public:
	virtual ~AbstractFilter() {}
};

class AbstractSorter {
public:
	virtual ~AbstractSorter() {}
};

} // namespace Dataset
} // namespace Vizzu
#endif // DATAFRAME_TYPES_H
