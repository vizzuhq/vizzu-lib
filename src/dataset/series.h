#ifndef DATASET_SERIES_H
#define DATASET_SERIES_H

#include "types.h"

namespace Vizzu
{
namespace DataSet
{

class AbstractSeries {
public:
    virtual int size() const = 0;
    virtual Value& at(int index) = 0;
    virtual Value at(int index) const = 0;
    virtual ValueIterator begin() const = 0;
    virtual ValueIterator end() const = 0;
};

class RawSeries : public AbstractSeries {
public:
    RawSeries();

    int size() const override;
    void resize(int newSize);
    Value& at(int index) override;
    Value at(int index) const override;
    ValueIterator begin() const override;
    ValueIterator end() const override;

protected:
    SeriesId id;
    std::string name;
    ValueVector values;
};

class Series : public AbstractSeries {
public:
    Series();

    ValueType type() const;
    void normalize(ValueType type);
    int size() const override;
    Value& at(int index) override;
    ValueIterator begin() const override;
    ValueIterator end() const override;

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
    SeriesPtr newSeries();
    SeriesPtr getSeries(SeriesId id);
    SeriesPtr getSeries(const char* name);

protected:
    SeriesMap series;
};

}
}

#endif //DATASET_SERIES_H
