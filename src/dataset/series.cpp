#include "dataset.h"
#include "series.h"
#include "value.h"
#include "iterators.h"
#include "mutableseries.h"
#include "range.h"

namespace Vizzu {
namespace Dataset {

SeriesId SeriesContainer::nextSeriesId = SeriesContainer::nullId;

Series::Series() {
}

ValueType Series::type() const {
    return ValueType::null;
}

ValueType Series::typeAt(int) const {
    return ValueType::null;
}

Value Series::valueAt(int) const {
    return Value{};
}

int Series::size() const {
    return 0;
}

ValueIterator Series::begin() const {
    return ValueIterator{};
}

ValueIterator Series::end() const {
    return ValueIterator{};
}

RangePtr Series::range() const {
    return RangePtr{};
}

SeriesIndex::SeriesIndex() {
}

int SeriesIndex::size() const {
    return 0;
}

void SeriesIndex::resize(int) {
}

int& SeriesIndex::at(int) {
    static int empty = 0;
    return empty;
}

ValueIterator SeriesIndex::begin() const {
    return ValueIterator{};
}

ValueIterator SeriesIndex::end() const {
    return ValueIterator{};
}

SeriesContainer::SeriesContainer(Dataset& dataset)
    : dataset(dataset)
{
}

int SeriesContainer::size() const {
    return series.size();
}

void SeriesContainer::clear() {
}

SeriesIterator SeriesContainer::begin() const {
    return series.begin();
}

SeriesIterator SeriesContainer::end() const {
    return series.end();
}

SeriesPtr SeriesContainer::getSeries(SeriesId id) const {
    auto iter = series.find(id);
    if (iter == series.end())
        return SeriesPtr{};
    return iter->second;
}

SeriesPtr SeriesContainer::getSeries(const char* name) const {
    for(auto& s : series) {
        if (s.second->name() == name)
            return s.second;
    }
    return SeriesPtr{};
}

SeriesId SeriesContainer::insert(SeriesPtr ptr) {
    nextSeriesId++;
    series.insert(std::make_pair(nextSeriesId, ptr));
    return nextSeriesId;
}

}
}
