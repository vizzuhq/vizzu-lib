#include "dataset.h"
#include "value.h"
#include "linkedseries.h"

namespace Vizzu {
namespace Dataset {

LinkedSeries::LinkedSeries(const ConstantSeriesPtr& series) :
    BaseSeries(series->owner(), series->name()), series(series)
{
}

LinkedSeries::LinkedSeries(const ConstantSeriesPtr& series, const char* name) :
    BaseSeries(series->owner(), name)
{
}

int LinkedSeries::size() const {
    return selector && selector->size() ? selector->size() : series->size();
}

Dataset& LinkedSeries::owner() const {
    return series->owner();
}

DatasetId LinkedSeries::id() const {
    return seriesId;
}

const char* LinkedSeries::name() const {
    return seriesName.c_str();
}

ValueType LinkedSeries::type() const {
    return series->type();
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

int LinkedSeries::originalSize() const {
    return series ? series->size() : 0;
}

const ConstantSeriesPtr& LinkedSeries::originalSeries() {
    return series;
}

void LinkedSeries::setSelector(const indices_ptr& sel) {
    selector = sel;
}

LinkedSeries::indices_ptr LinkedSeries::getSelector() const {
    return selector;
}

LinkedSeries::indices_ptr LinkedSeries::createSelector(int size) {
    selector = std::make_shared<indices>();
    selector->reserve(size);
    return selector;
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
