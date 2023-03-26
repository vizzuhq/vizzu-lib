#include <string.h>

#include "types.h"
#include "dataset.h"
#include "series/rawseries.h"
#include "base/conv/numtostr.h"

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
    converter = [&](Dataset&, ValueType type, Value value) -> Value {
        if (type == ValueType::continous)
            return getValue(std::to_string(value.getc()).c_str());
        else if (type == ValueType::discrete)
            return getValue(atof(value.getd().value()));
        return Value{};
    };
}

Dataset::Dataset(const Dataset& src)
{
    converter = src.converter;
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
    src.enumSeriesAs<RawSeries>([&](RawSeries& inst) {
        newSeries<RawSeries>(inst.name(), inst);
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
    if (name && name[0] == 'T' && name[1] == '(') {
        std::string tableName;
        auto end = strchr(name + 2, ')');
        if (end != nullptr) {
            tableName = std::string{name + 2, (size_t)(end - name - 2)};
            auto table = getTable(tableName.c_str());
            if (table)
                return table->getSeries(end + 1);
        }
    }
    else {
        auto iter = seriesByName.find(name);
        if (iter != seriesByName.end())
            return iter->second;
    }
    return ConstantSeriesPtr{};
}

void Dataset::deleteSeries(const char* name) {
    seriesByName.erase(name);
    deleteRange(name);
}

RangePtr Dataset::getRange(const char* name) {
    auto iter = rangesByName.find(name);
    if (iter != rangesByName.end())
        return iter->second;
    auto siter = seriesByName.find(name);
    if (siter == seriesByName.end())
        return RangePtr{};
    auto result = rangesByName.insert(
        std::make_pair(siter->second->name(),
        std::make_shared<Range>(siter->second)));
    return result.first->second;
}

void Dataset::deleteRange(const char* name) {
    auto iter = rangesByName.find(name);
    if (iter != rangesByName.end())
        rangesByName.erase(iter);
}

const TableContainer& Dataset::tables() const {
    return tablesByName;
}

const SeriesContainer& Dataset::series() const {
    return seriesByName;
}

}
}
