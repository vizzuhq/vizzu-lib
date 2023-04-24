#ifndef DATASET_FILTERS_H
#define DATASET_FILTERS_H

#include "../types.h"
#include "../interface.h"

namespace Vizzu {
namespace Dataset {
namespace Filters {

class ByValue : public AbstractFilter {
public:
    ByValue(const char* seriesName, const char* discreteValue);
    ByValue(const char* seriesName, double continousValue);

    void setup(const Dataset& ds) override;
    void setup(const ConstantTablePtr& table) override;
    bool filterRecord(int recordIndex) override;

protected:
    std::string name;
    Value value;
    ValueType type;
};

class ByRange : public AbstractFilter {
public:
    ByRange(const char* seriesName, double cValMin, double cValMax);

    void setup(const Dataset& ds) override;
    void setup(const ConstantTablePtr& table) override;
    bool filterRecord(int recordIndex) override;

protected:
    std::string name;
    double minimunValue;
    double maximumValue;
    ConstantSeriesPtr series;
};

class ByRecord : public AbstractFilter {
public:
    class RecordValue {
    public:
        Value value;
        ValueType type;
    };

    using record = std::vector<RecordValue>;
    using filterFn = std::function<bool(const record&)>;

public:
    ByRecord(filterFn filter);

    void setup(const Dataset& ds) override;
    void setup(const ConstantTablePtr& table) override;
    bool filterRecord(int recordIndex) override;

protected:
    filterFn filter;
};

}
}
}

#endif // DATASET_FILTERS_H
