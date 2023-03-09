#include "dataset.h"
#include "series.h"
#include "value.h"
#include "iterators.h"
#include "mutableseries.h"
#include "tablefilters.h"

namespace Vizzu {
namespace Dataset {
namespace Filters {

ByValue::ByValue(const char*, const char*) {
}

ByValue::ByValue(const char*, double, double) {
}

ByRecord::ByRecord(filterFn) {
}


}
}
}
