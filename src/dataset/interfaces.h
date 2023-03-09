#ifndef DATAFRAME_INTERFACES_H
#define DATAFRAME_INTERFACES_H

#include "types.h"

namespace Vizzu
{
namespace Dataset
{

/**
 * @brief The AbstractSeries intrface
 * @details This class is the base class for all Series
 * implementations. It grants the ability to iterate over the values
 * of a Series and to get the value at a given index.
 */
class AbstractSeries
{
public:
	virtual ~AbstractSeries() {}
	virtual int size() const = 0;
	virtual SeriesId id() const = 0;
	virtual const std::string &name() const = 0;
	virtual ValueType type() const = 0;
	virtual ValueType typeAt(int index) const = 0;
	virtual Value valueAt(int index) const = 0;
	virtual ValueIterator begin() const = 0;
	virtual ValueIterator end() const = 0;
	virtual RangePtr range() const = 0;
};

class AbstractTableGenerator {
public:
    virtual ~AbstractTableGenerator() {};
};

class AbstractSeriesGenerator {
public:
    virtual ~AbstractSeriesGenerator() {};
};

class AbstractSeriesAggregator {
public:
    virtual ~AbstractSeriesAggregator() {};
};

} // namespace Dataset
} // namespace Vizzu
#endif // DATAFRAME_TYPES_H
