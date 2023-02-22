
#include "series.h"
#include "value.h"
#include "valueiterator.h"

namespace Vizzu {
namespace DataSet {

Series::Series() {
}

ValueType Series::type() const {
    return ValueType::null;
}

int Series::size() const {
    return 0;
}

const Value& Series::at(int) const {
    static Value empty;
    return empty;
}

ValueIterator Series::begin() const {
    return ValueIterator{};
}

ValueIterator Series::end() const {
    return ValueIterator{};
}

void Series::normalize(ValueType) {
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

SeriesContainer::SeriesContainer() {
}

int SeriesContainer::size() {
    return 0;
}

SeriesIterator SeriesContainer::begin() {
    return SeriesIterator{};
}

SeriesIterator SeriesContainer::end() {
    return SeriesIterator{};
}

SeriesPtr SeriesContainer::getSeries(SeriesId) {
    return SeriesPtr{};
}

SeriesPtr SeriesContainer::getSeries(const char*) {
    return SeriesPtr{};
}

MutableSeriesPtr SeriesContainer::newMutableSeries(const char*) {
    return MutableSeriesPtr{};
}

}
}
