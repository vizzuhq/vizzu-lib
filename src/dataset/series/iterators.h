#ifndef DATASET_VALUEITERATOR_H
#define DATASET_VALUEITERATOR_H

#include "../types.h"

namespace Vizzu
{
namespace Dataset
{

template<class S>
class IndexBasedIterator {
public:
    IndexBasedIterator();
    IndexBasedIterator(int index, S series);

    Value value() const;
    ValueType type() const;
    IndexBasedIterator& operator++();
    IndexBasedIterator operator++(int);
    IndexBasedIterator& operator--();
    IndexBasedIterator operator--(int);
    IndexBasedIterator& operator+=(int arg);
    IndexBasedIterator operator+(int arg) const;
    IndexBasedIterator& operator-=(int arg);
    IndexBasedIterator operator-(int arg) const;
    bool operator==(const IndexBasedIterator& arg) const;
    bool operator!=(const IndexBasedIterator& arg) const;
    Value operator*() const;
    explicit operator int();

protected:
    int index;
    S series;
};

#include "iterators.hpp"

}
}

#endif //DATASET_VALUEITERATOR_H
