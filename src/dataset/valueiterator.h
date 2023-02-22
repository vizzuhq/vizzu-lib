#ifndef DATASET_VALUEITERATOR_H
#define DATASET_VALUEITERATOR_H

#include "types.h"

namespace Vizzu
{
namespace DataSet
{

/**
 * @brief The ValueIterator class
 * @details This class is used to iterate over the values of a Series.
 *      It is used by all AbstractSeries descendants.
 *      The iterator supports only constant value access.
 */
class ValueIterator {
friend class Series;
friend class MutableSeries;
public:
    ValueIterator();
    ValueIterator& operator++();
    ValueIterator operator++(int) const;
    ValueIterator& operator--();
    ValueIterator operator--(int) const;
    ValueIterator& operator+=(int arg);
    ValueIterator operator+(int arg) const;
    ValueIterator& operator-=(int arg);
    ValueIterator operator-(int arg) const;
    bool operator==(const ValueIterator& arg) const;
    Value operator*() const;
    const Value* operator->() const;

protected:
    int index;
    ConstSeriesPtr series;

    ValueIterator(int index, const ConstSeriesPtr& series);
};

}
}

#endif //DATASET_VALUEITERATOR_H
