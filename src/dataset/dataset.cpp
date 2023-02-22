#include "types.h"
#include "dataset.h"
#include "mutableseries.h"

namespace Vizzu::DataSet {

bool DiscreteValueComparer::operator()(const char*, const char*) const {
    return false;
}

bool DiscreteValueComparer::operator()(const DiscreteValue&, const DiscreteValue&) const {
    return false;
}

size_t DiscreteValueHasher::operator()(const char*) const {
    return 0;
}

size_t DiscreteValueHasher::operator()(const DiscreteValue&) const {
    return 0;
}

/**
 * Implementation of Dataset class
 */
DataSet::DataSet()
{
}

Value DataSet::getValue(double cval) {
    return Value(cval);
}

Value DataSet::getValue(const char* dval) {
    return Value(discreteValues.get(dval));
}

MutableSeriesPtr DataSet::getMutableSeries(const char* name) {
    auto sptr = series.getSeries(name);
    if (sptr != SeriesPtr{})
        return std::dynamic_pointer_cast<MutableSeries>(sptr);
    return MutableSeriesPtr{};
}

MutableSeriesPtr DataSet::makeMutableSeries(const char* name) {
    auto sptr = series.getSeries(name);
    if (sptr == SeriesPtr{})
        return series.newMutableSeries(name);
    return MutableSeriesPtr{};
}

}