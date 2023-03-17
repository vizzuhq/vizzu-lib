#include "dataset.h"
#include "value.h"
#include "iterators.h"
#include "originalseries.h"
#include "aggregators.h"

namespace Vizzu {
namespace Dataset {
namespace Aggregators {

void Count::setup(const Dataset&) {
}

ValueType Count::type() {
    return ValueType::continous;
}

Value Count::calculate(RangeIndexIterator&, RangeIndexIterator&) {
    return Value{};
}

Min::Min(const char*) {
}

void Min::setup(const Dataset&) {
}

ValueType Min::type() {
    return ValueType::continous;
}

Value Min::calculate(RangeIndexIterator& from, RangeIndexIterator& to) {
    int count = 0;
    for(; from != to; from++, count++);
    return Value{count};
}

Max::Max(const char*) {
}

void Max::setup(const Dataset&) {
}

ValueType Max::type() {
    return ValueType::continous;
}

Value Max::calculate(RangeIndexIterator&, RangeIndexIterator&) {
    return Value{};
}

Avarage::Avarage(const char*) {
}

void Avarage::setup(const Dataset&) {
}

ValueType Avarage::type() {
    return ValueType::continous;
}

Value Avarage::calculate(RangeIndexIterator&, RangeIndexIterator&) {
    return Value{};
}

}
}
}
