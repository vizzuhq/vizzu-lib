#ifndef DATASET_AGGREGATORS_H
#define DATASET_AGGREGATORS_H

#include "types.h"
#include "interfaces.h"

namespace Vizzu {
namespace Dataset {
namespace Aggregators {

class Count : public AbstractSeriesAggregator {
    void setup(const Dataset& ds) override;
    ValueType type() override;
    Value calculate(RangeIndexIterator& from, RangeIndexIterator& to) override;
};

class Min : public AbstractSeriesAggregator {
public:
    Min(const char* datasetSeriesName);
    void setup(const Dataset& ds) override;
    ValueType type() override;
    Value calculate(RangeIndexIterator& from, RangeIndexIterator& to) override;
};

class Max : public AbstractSeriesAggregator {
public:
    Max(const char* datasetSeriesName);
    void setup(const Dataset& ds) override;
    ValueType type() override;
    Value calculate(RangeIndexIterator& from, RangeIndexIterator& to) override;
};

class Avarage : public AbstractSeriesAggregator {
public:
    Avarage(const char* datasetSeriesName);
    void setup(const Dataset& ds) override;
    ValueType type() override;
    Value calculate(RangeIndexIterator& from, RangeIndexIterator& to) override;
};

}
}
}

#endif // DATASET_AGGREGATORS_H
