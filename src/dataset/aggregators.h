#ifndef DATASET_AGGREGATORS_H
#define DATASET_AGGREGATORS_H

#include "types.h"
#include "interfaces.h"

namespace Vizzu {
namespace Dataset {
namespace Aggregators {

class Count : public AbstractSeriesAggregator {
};

class Min : public AbstractSeriesAggregator {
public:
    Min(const char* datasetSeriesName);
};

class Max : public AbstractSeriesAggregator {
public:
    Max(const char* datasetSeriesName);
};

class Avarage : public AbstractSeriesAggregator {
public:
    Avarage(const char* datasetSeriesName);
};

}
}
}

#endif // DATASET_AGGREGATORS_H
