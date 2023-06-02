#include <string.h>

#include "../types.h"
#include "../dataset.h"
#include "baseseries.h"

namespace Vizzu {
namespace Dataset {

BaseSeries::BaseSeries(Dataset& dataset)
    : dataset(dataset), seriesId(nullid), seriesName(""), seriesType(ValueType::null)
{
}

BaseSeries::BaseSeries(Dataset& dataset, const char* name)
    : dataset(dataset), seriesId(nullid), seriesName(name), seriesType(ValueType::null)
{
    seriesId = DiscreteValue::hash(name);
}

double BaseSeries::calculateTypeRate(const AbstractConstantSeries& series, ValueType type) const {
    double count = 0;
    if (seriesType == ValueType::null) {
        for(int i = 0; i < series.size(); i++) {
            if (series.typeAt(i) == type)
                count++;
        }
    }
    else if (type == seriesType)
        count = series.size();
    return count / series.size();
}

}
}
