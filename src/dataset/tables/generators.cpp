#include "../dataset.h"
#include "../series/value.h"
#include "../series/iterators.h"
#include "../series/rawseries.h"
#include "generators.h"

namespace Vizzu {
namespace Dataset {
namespace Generators {

void Ordinal::setup(const Dataset&) {
}

ValueType Ordinal::getType(int) {
    return ValueType::continous;
}

Value Ordinal::getValue(int record) {
    return Value{record};
}

}
}
}
