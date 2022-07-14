#include "types.h"
#include "dataset.h"
#include "mutableseries.h"

using namespace Vizzu::DataSet;

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
