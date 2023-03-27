#ifndef DATASET_AGGREGATORS_H
#define DATASET_AGGREGATORS_H

#include "../types.h"
#include "../interface.h"

namespace Vizzu {
namespace Dataset {
namespace Aggregators {

class Count : public AbstractSeriesAggregator {
public:
    void setup(const Dataset& ds) override;
    void aggregateRecord(int index) override;
    Value aggregatedValue() override;
    ValueType aggregatedType() override;

protected:
    int count;
};

class Min : public AbstractSeriesAggregator {
public:
    Min(const char* datasetSeriesName);
    void setup(const Dataset& ds) override;
    void aggregateRecord(int index) override;
    Value aggregatedValue() override;
    ValueType aggregatedType() override;

protected:
    bool firstValue;
    double minimum;
    std::string seriesName;
    ConstantSeriesPtr series;
};

class Max : public AbstractSeriesAggregator {
public:
    Max(const char* datasetSeriesName);
    void setup(const Dataset& ds) override;
    void aggregateRecord(int index) override;
    Value aggregatedValue() override;
    ValueType aggregatedType() override;

protected:
    bool firstValue;
    double maximum;
    std::string seriesName;
    ConstantSeriesPtr series;
};

class Avarage : public AbstractSeriesAggregator {
public:
    Avarage(const char* datasetSeriesName);
    void setup(const Dataset& ds) override;
    void aggregateRecord(int index) override;
    Value aggregatedValue() override;
    ValueType aggregatedType() override;

protected:
    double sum;
    int count;
    std::string seriesName;
    ConstantSeriesPtr series;
};

}
}
}

#endif // DATASET_AGGREGATORS_H
