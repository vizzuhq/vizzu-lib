#include "../dataset.h"
#include "../series/value.h"
#include "../series/iterators.h"
#include "filters.h"

namespace Vizzu {
namespace Dataset {
namespace Filters {

ByValue::ByValue(const char*, const char*) {
}

ByValue::ByValue(const char*, double) {
}

void ByValue::setup(const Dataset&) {
}

bool ByValue::filterRecord(int) {
    return false;
}

ByRange::ByRange(const char* name, double minVal, double maxVal) :
    name(name), minimunValue(minVal), maximumValue(maxVal)
{
}

void ByRange::setup(const Dataset& ds) {
    series = ds.getSeries(name.c_str());
    if (!series)
        throw "unknown series";
    if (series->type() != ValueType::continous)
        throw "continous series required";
}

bool ByRange::filterRecord(int recordIndex) {
    auto value = series->valueAt(recordIndex).getc();
    return value >= minimunValue && value <= maximumValue;
}

ByRecord::ByRecord(filterFn) {
}

void ByRecord::setup(const Dataset&) {
}

bool ByRecord::filterRecord(int) {
    return false;
}

}
}
}
