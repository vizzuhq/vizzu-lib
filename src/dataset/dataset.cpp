#include <string.h>

#include "types.h"
#include "dataset.h"
#include "mutableseries.h"

namespace Vizzu::DataSet {

bool DiscreteValueComparer::operator()(const char* op1, const char* op2) const {
    return strcmp(op1, op2) == 0;
}

bool DiscreteValueComparer::operator()(const DiscreteValue& op1, const DiscreteValue& op2) const {
    return op1 == op2;
}

size_t DiscreteValueHasher::operator()(const char* op) const {
    return DiscreteValue::hash(op);
}

size_t DiscreteValueHasher::operator()(const DiscreteValue& op) const {
    return op.hash();
}

/**
 * Implementation of Dataset class
 */
DataSet::DataSet()
    : series(*this)
{
}

Value DataSet::getValue(double cval) {
    return Value(cval);
}

Value DataSet::getValue(const char* dval) {
    return Value(discreteValues.get(dval));
}

const DiscreteValueContainer& DataSet::values() const {
    return discreteValues;
}

MutableSeriesPtr DataSet::getMutableSeries(const char* name) {
    auto sptr = series.getSeries(name);
    if (sptr != SeriesPtr{})
        return std::dynamic_pointer_cast<MutableSeries>(sptr);
    return MutableSeriesPtr{};
}

MutableSeriesPtr DataSet::makeMutableSeries(const char* name) {
    auto sptr = series.getSeries(name);
    if (!sptr) {
        MutableSeriesPtr ptr;
        ptr.reset(new MutableSeries(*this, SeriesContainer::nullId, name));
        SeriesId id = series.insert(ptr);
        ptr->seriesId = id;
        return ptr;        
    }
    return std::dynamic_pointer_cast<MutableSeries>(sptr);
}

const SeriesContainer& DataSet::mutableSeries() const {
    return series;
}

}