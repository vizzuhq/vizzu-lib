#include "types.h"
#include "series.h"
#include "valueiterator.h"
#include "value.h"

using namespace Vizzu::DataSet;

/**
 * Implementation of ValueIterator class
 */
ValueIterator::ValueIterator()
    : index(0)
{
}

ValueIterator::ValueIterator(int index, const ConstSeriesPtr& series)
    : index(index), series(series)
{
    if (index < 0)
        index = 0;
    else if (index > series->size())
        index = series->size();
}

ValueIterator& ValueIterator::operator++() {
    if (index == series->size())
        index = 0;
    else
        index++;
    return *this;
}

ValueIterator ValueIterator::operator++(int) const {
    return ++ValueIterator{index, series};
}

ValueIterator& ValueIterator::operator--() {
    if (index == 0)
        index = series->size();
    else
        index--;
    return *this;
}

ValueIterator ValueIterator::operator--(int) const {
    return --ValueIterator{index, series};
}

ValueIterator& ValueIterator::operator+=(int arg) {
    index += arg;
    if (index < 0)
        index = 0;
    else if (index > series->size())
        index = series->size();
    return *this;
}

ValueIterator ValueIterator::operator+(int arg) const {
    return ValueIterator{index + arg, series};
}

ValueIterator& ValueIterator::operator-=(int arg) {
    index -= arg;
    if (index < 0)
        index = 0;
    else if (index > series->size())
        index = series->size();
    return *this;
}

ValueIterator ValueIterator::operator-(int arg) const {
    return ValueIterator{index - arg, series};
}

bool ValueIterator::operator==(const ValueIterator& arg) const {
    return index == arg.index && series == arg.series;
}

Value ValueIterator::operator*() const {
    return series->at(index);
}

const Value* ValueIterator::operator->() const {
    return &series->at(index);
}
