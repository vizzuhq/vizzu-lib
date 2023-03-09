#ifndef DATASET_SORTERS_H
#define DATASET_SORTERS_H

#include "types.h"
#include "interfaces.h"

namespace Vizzu {
namespace Dataset {
namespace Sorters {

class SingleColumn : public AbstractSorter {
public:
    SingleColumn(const char* seriesName);
};

class MultiColumn : public AbstractSorter {
public:
    MultiColumn(const char* seriesName);
};

}
}
}

#endif // DATASET_SORTERS_H
