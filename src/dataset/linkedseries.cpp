#include "dataset.h"
#include "value.h"
#include "iterators.h"
#include "range.h"
#include "linkedseries.h"

namespace Vizzu {
namespace Dataset {

LinkedSeries::LinkedSeries(const ConstantSeriesPtr& series, const char* name) :
    BaseSeries(series->owner(), name)
{
}

int LinkedSeries::size() const {
    return series->size();
}

DatasetId LinkedSeries::id() const {
    return seriesId;
}

const char* LinkedSeries::name() const {
    return seriesName.c_str();
}

ValueType LinkedSeries::type() const {
    return seriesType;
}

ValueType LinkedSeries::typeAt(int index) const {
    if (selector)
        return series->typeAt(selector->at(index));
    return series->typeAt(index);
}

Value LinkedSeries::valueAt(int index) const {
    if (selector)
        return series->valueAt(selector->at(index));
    return series->valueAt(index);
}

ValueIterator LinkedSeries::begin() const {
    return ValueIterator{0, this};
}

ValueIterator LinkedSeries::end() const {
    return ValueIterator{size(), this};
}

void LinkedSeries::setSelector(const selection_vector_ptr& sel) {
    selector = sel;
}

LinkedSeries::selection_vector_ptr LinkedSeries::getSelector() const {
    return selector;
}

void LinkedSeries::createSelector(int size) {
    selector = std::make_shared<selection_vector>();
    selector->reserve(size);
}

void LinkedSeries::releaseSelector() {
    selector.reset();
}

void LinkedSeries::select(int recordIndex) {
    selector->push_back(recordIndex);
}

void LinkedSeries::deselect(int recordIndex) {
    for(auto i = selector->begin(); i != selector->end();) {
        if (*i == recordIndex)
            i = selector->erase(i);
        else
            i++;
    }
}

}
}
