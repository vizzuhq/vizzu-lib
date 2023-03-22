#ifndef DATASET_BASESERIES_H
#define DATASET_BASESERIES_H

#include "../types.h"

namespace Vizzu {
namespace Dataset {

class BaseSeries {
public:
    BaseSeries(Dataset& dataset);
    BaseSeries(Dataset& dataset, const char* name);

protected:
    Dataset& dataset;
    DatasetId seriesId;
    std::string seriesName;
    ValueType seriesType;

    double calculateTypeRate(const AbstractConstantSeries& series, ValueType type) const;
};

}
}

#endif //DATASET_BASESERIES_H
