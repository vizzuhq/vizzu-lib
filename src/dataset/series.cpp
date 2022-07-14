#ifndef DATASET_SERIES_H
#define DATASET_SERIES_H

#include "types.h"

namespace Vizzu
{
namespace DataSet
{

/**
 * @brief The AbstractSeries intrface
 * @details This class is the base class for all Series implementations.
 *     It grants the ability to iterate over the values of a Series and
 *     to get the value at a given index.
 */
class AbstractSeries {
public:
    virtual int size() const = 0;
    virtual SeriesId id() const = 0;
    virtual const std::string& name() const = 0;
    virtual ValueType type() const = 0;
    virtual Value at(int index) const = 0;
    virtual ValueIterator begin() const = 0;
    virtual ValueIterator end() const = 0;
};

class Series : public AbstractSeries {
public:
    Series();

    ValueType type() const;
    int size() const override;
    Value at(int index) const override;
    ValueIterator begin() const override;
    ValueIterator end() const override;

    void normalize(ValueType type);

protected:
    SeriesId id;
    ValueType vtype;
    SeriesIndexPtr index;
    SeriesPtr data;
    SeriesInfo info;
};

class SeriesIndex {
public:
    SeriesIndex();

    int size() const;
    void resize(int newSize);
    int& at(int arg);
    ValueIterator begin() const;
    ValueIterator end() const;

protected:
    IndexVector forwardIndices;
    IndexVector backwardIndices;
};

class SeriesContainer {
public:
    SeriesContainer();

    int size();
    SeriesIterator begin();
    SeriesIterator end();
    SeriesPtr getSeries(SeriesId id);
    SeriesPtr getSeries(const char* name);
    MutableSeriesPtr newMutableSeries(const char* name);

protected:
    SeriesMap series;
    uint32_t nextSeriesId;
};

}
}

#endif //DATASET_SERIES_H
