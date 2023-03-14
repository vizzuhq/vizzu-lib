#include "dataset.h"
#include "value.h"
#include "iterators.h"
#include "tablefilters.h"

namespace Vizzu {
namespace Dataset {
namespace Filters {

ByValue::ByValue(const char*, const char*) {
}

ByValue::ByValue(const char*, double) {
}

ByRange::ByRange(const char*, double, double) {
}

ByRecord::ByRecord(filterFn) {
}


}
}
}
