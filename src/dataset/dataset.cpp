#include <string.h>

#include "types.h"
#include "dataset.h"
#include "originalseries.h"

namespace Vizzu {
namespace Dataset {

bool DiscreteValueComparer::operator()(const char* op1, const char* op2) const {
    return strcmp(op1, op2) == 0;
}

bool DiscreteValueComparer::operator()(const DiscreteValue& op1, const DiscreteValue& op2) const {
    return op1 == op2;
}

size_t DiscreteValueHasher::operator()(const char* op) const {
    return DiscreteValue::hash(op);
}

size_t DiscreteValueHasher::operator()(const DiscreteValue& op) const {
    return op.hash();
}

bool ConstCharPtrComparator::operator()(const char* a, const char* b) const {
    return strcmp(a, b) < 0;
}

/**
 * Implementation of Dataset class
 */
Dataset::Dataset() {
    C2DConverter = [=](const AbstractConstantSeries&, double cv) -> std::string {
        return std::to_string(cv);
    };
    D2CConverter = [=](const AbstractConstantSeries&, const char* str) -> double {
        return atof(str);
    };
}

Dataset::Dataset(const Dataset& src)
{
    C2DConverter = src.C2DConverter;
    D2CConverter = src.D2CConverter;
    deepCopy(src);
}

void Dataset::clear() {
    tablesByName.clear();
    seriesByName.clear();
    values.clear();
}

bool Dataset::empty() {
    return seriesByName.size() == 0;
}

void Dataset::compact() {
}

void Dataset::deepCopy(const Dataset& src) {
    src.enumSeriesAs<OriginalSeries>([&](OriginalSeries& inst) {
        newSeries<OriginalSeries>(inst.name(), inst);
    });
}

Value Dataset::getValue(double cval) {
    return Value(cval);
}

Value Dataset::getValue(const char* dval) {
    return Value(values.get(dval));
}

const DiscreteValueContainer& Dataset::discreteValues() const {
    return values;
}

ConstantTablePtr Dataset::getTable(const char* name) const {
    auto iter = tablesByName.find(name);
    if (iter != tablesByName.end())
        return iter->second;
    return ConstantTablePtr{};
}

void Dataset::deleteTable(const char* name) {
    tablesByName.erase(name);
}

ConstantSeriesPtr Dataset::getSeries(const char* name) const {
    auto iter = seriesByName.find(name);
    if (iter != seriesByName.end())
        return iter->second.series;
    return ConstantSeriesPtr{};
}

void Dataset::deleteSeries(const char* name) {
    seriesByName.erase(name);
}

RangePtr Dataset::getRange(const char* name) {
    auto iter = seriesByName.find(name);
    if (iter != seriesByName.end())
        return iter->second.range;
    iter->second.range = std::make_shared<Range>(iter->second.series);
    return iter->second.range;
}

void Dataset::deleteRange(const char* name) {
    auto iter = seriesByName.find(name);
    if (iter != seriesByName.end())
        iter->second.range.reset();
}

const TableContainer& Dataset::tables() const {
    return tablesByName;
}

const SeriesContainer& Dataset::series() const {
    return seriesByName;
}

}
}
