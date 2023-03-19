#include "dataset.h"
#include "value.h"
#include "iterators.h"
#include "originalseries.h"
#include "generators.h"

namespace Vizzu {
namespace Dataset {
namespace Generators {

void Ordinal::setup(const Dataset&) {
}

ValueType Ordinal::type() {
    return ValueType::continous;
}

Value Ordinal::generateRecord(int record) {
    return Value{record};
}

}
}
}
