#include <string.h>

#include "types.h"
#include "dataset.h"
#include "mutableseries.h"

namespace Vizzu::Dataset {

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

/**
 * Implementation of Dataset class
 */
Dataset::Dataset()
    : series(*this)
{
    C2DConverter = [=](const MutableSeriesPtr&, double cv) -> std::string {
        return std::to_string(cv);
    };
    D2CConverter = [=](const MutableSeriesPtr&, const char* str) -> double {
        return atof(str);
    };
}

Dataset::Dataset(const Dataset& src) 
    : series(*this)
{
    C2DConverter = src.C2DConverter;
    D2CConverter = src.D2CConverter;
    deepCopy(src);
}

void Dataset::clear() {
    tables.clear();
    series.clear();
    values.clear();
}

bool Dataset::empty() {
    return series.size() == 0;
}

void Dataset::compact() {
}

void Dataset::deepCopy(const Dataset& src) {
    for(auto& iter : src.mutableSeries()) {
        auto dsts = addMutableSeries(iter.second->name().c_str());
        auto srcs = std::dynamic_pointer_cast<MutableSeries>(iter.second);
        dsts->select(srcs->type());
        auto viter = srcs->begin();
        while(viter != srcs->end()) {
            if (viter.type() == ValueType::continous)
                dsts->insert(viter.value().getc(), nullpos);
            else
                dsts->insert(viter.value().getd().value(), nullpos);
            viter++;
        }
    }
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

MutableSeriesPtr Dataset::getMutableSeries(const char* name) {
    auto sptr = series.getSeries(name);
    if (sptr != SeriesPtr{})
        return std::dynamic_pointer_cast<MutableSeries>(sptr);
    return MutableSeriesPtr{};
}

MutableSeriesPtr Dataset::addMutableSeries(const char* name) {
    auto sptr = series.getSeries(name);
    if (!sptr) {
        MutableSeriesPtr ptr;
        ptr.reset(new MutableSeries(*this, SeriesContainer::nullId, name));
        SeriesId id = series.insert(ptr);
        ptr->seriesId = id;
        return ptr;        
    }
    return std::dynamic_pointer_cast<MutableSeries>(sptr);
}

TablePtr Dataset::addTable(const char* name) {
    auto table = std::make_shared<Table>(*this, name);
    tables.insertTable(table);
    return table;
}

TablePtr Dataset::addTable(const char* name, TableBuilderPtr builder) {
    auto table = std::make_shared<Table>(*this, name, builder);
    tables.insertTable(table);
    return table;
}

const SeriesContainer& Dataset::mutableSeries() const {
    return series;
}

}