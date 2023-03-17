#ifndef DATASET_GENERATORS_H
#define DATASET_GENERATORS_H

#include "types.h"
#include "interfaces.h"

namespace Vizzu {
namespace Dataset {
namespace Generators {

class Ordinal : public AbstractSeriesGenerator {
    void setup(const Dataset& ds) override;
    ValueType type() override;
    Value calculate() override;
};

}
}
}

#endif // DATASET_GENERATORS_H
