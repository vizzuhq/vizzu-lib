#ifndef DATASET_FILTERS_H
#define DATASET_FILTERS_H

#include "types.h"
#include "interfaces.h"

namespace Vizzu {
namespace Dataset {
namespace Filters {

class ByValue : public AbstractFilter {
public:
    ByValue(const char* seriesName, const char* discreteValue);
    ByValue(const char* seriesName, double continousValue);
};

class ByRange : public AbstractFilter {
public:
    ByRange(const char* seriesName, double cValMin, double cValMax);
};

class ByRecord : public AbstractFilter {
public:
    class Record {
        int getRecordIndex() const;
        SeriesId getSeriesId(const char* seriesName) const;
        Value getValue(SeriesId seriesId) const;
        Value getValue(const char* seriesName) const;
        ValueType getValueType(SeriesId seriesId) const;
        ValueType getValueType(const char* seriesName) const;
    };

    using filterFn = std::function<bool(const Record&)>;

public:
    ByRecord(filterFn filterFunction);
};

}
}
}

#endif // DATASET_FILTERS_H
