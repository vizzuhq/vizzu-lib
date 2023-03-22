#include "../dataset.h"
#include "value.h"
#include "iterators.h"
#include "rawseries.h"
#include "range.h"

using namespace Vizzu::Dataset;

RawSeries::RawSeries(Dataset& dataset)
    : BaseSeries(dataset)
{
}

RawSeries::RawSeries(Dataset& dataset, const char* name)
    : BaseSeries(dataset, name)
{
}

RawSeries::RawSeries(const RawSeries& src)
    : BaseSeries(src.dataset, src.seriesName.c_str())
{
    copy(src);
}

RawSeries::RawSeries(Dataset& dataset, const char* name, const RawSeries& src)
    : BaseSeries(dataset, name)
{
    copy(src);
}

Dataset& RawSeries::owner() const {
    return dataset;
}

int RawSeries::size() const {
    return values.size();
}

DatasetId RawSeries::id() const {
    return seriesId;
}

const char* RawSeries::name() const {
    return seriesName.c_str();
}

ValueType RawSeries::type() const {
    return seriesType;
}

Value RawSeries::valueAt(int index) const {
    try {
        return values[index];
    } catch (std::out_of_range&) {
        throw dataset_error("series index out of range");
    }
}

ValueType RawSeries::typeAt(int position) const {
    try {
        if (seriesType == ValueType::null)
            return vtypes[position];
        return seriesType;
    } catch (std::out_of_range&) {
        throw dataset_error("series index out of range");
    }
}

ValueIterator RawSeries::begin() const {
    return ValueIterator{0, this};
}

ValueIterator RawSeries::end() const {
    return ValueIterator{size(), this};
}

double RawSeries::typeRate(ValueType type) const {
    return calculateTypeRate(*this, type);
}

void RawSeries::selectType(ValueType type) {
    if (seriesType != ValueType::null && type == ValueType::null) {
        vtypes.reserve(values.size());
        for(int i = 0; i < (int)values.size(); i++)
            vtypes.push_back(seriesType);
        seriesType = ValueType::null;
    }
    else {
        for(int i = 0; i < (int)values.size(); i++) {
            if (typeAt(i) != type && type == ValueType::continous) {
                auto cval = dataset.D2CConverter(*this, values[i].getd().value());
                values[i] = dataset.getValue(cval);
            }
            if (typeAt(i) != type && type == ValueType::discrete) {
                auto dval = dataset.C2DConverter(*this, values[i].getc());
                values[i] = dataset.getValue(dval.c_str());
            }    
        }
        vtypes.clear();
        vtypes.shrink_to_fit();
        seriesType = type;
    }
}

void RawSeries::extend(int size) {
    try {
        values.reserve(size);
        if (seriesType == ValueType::null)
            vtypes.reserve(size);
    } catch (std::bad_alloc&) {
        throw dataset_error("series out of memory");
    }
}

void RawSeries::insert(double value, int pos) {
    insert(ValueType::continous, dataset.getValue(value), pos);
}

void RawSeries::insert(const char* value, int pos) {
    insert(ValueType::discrete, dataset.getValue(value), pos);
}

void RawSeries::insert(ValueType vt, const Value& value, int pos) {
    if (vt == ValueType::null)
        throw dataset_error("null value insertion");
    if (pos == nullpos)
        pos = values.size();
    auto type = seriesType;
    if (type == ValueType::null) {
        type = vt;
        vtypes.insert(vtypes.begin() + pos, vt);
    }
    if (vt == ValueType::discrete) {
        if (type == ValueType::discrete)
            values.insert(values.begin() + pos, value);
        else {
            auto conv = dataset.D2CConverter(*this, value.getd().value());
            values.insert(values.begin() + pos, dataset.getValue(conv));
        }
    }
    else {
        if (type == ValueType::continous)
            values.insert(values.begin() + pos, value);
        else {
            auto conv = dataset.C2DConverter(*this, value.getc());
            values.insert(values.begin() + pos, dataset.getValue(conv.c_str()));
        }
    }
}

void RawSeries::modify(double value, int pos) {
    modify(ValueType::continous, dataset.getValue(value), pos);
}

void RawSeries::modify(const char* value, int pos) {
    modify(ValueType::discrete, dataset.getValue(value), pos);
}

void RawSeries::modify(ValueType vt, const Value& value, int pos) {
    try {
        if (seriesType == ValueType::null)
            vtypes[pos] = vt;
        if (seriesType == ValueType::discrete && vt == ValueType::continous) {
            auto conv = dataset.D2CConverter(*this, value.getd().value());
            values[pos] = dataset.getValue(conv);
        }
        else if (seriesType == ValueType::continous && vt == ValueType::discrete) {
            auto conv = dataset.C2DConverter(*this, value.getc());
            values[pos] = dataset.getValue(conv.c_str());
        }
        else
            values[pos] = value;
    } catch (std::bad_alloc&) {
        throw dataset_error("series out of bound");
    }
}

void RawSeries::erase(int index) {
    try {
        values.erase(values.begin() + index);
        if (seriesType == ValueType::null)
            vtypes.erase(vtypes.begin() + index);
    } catch (std::bad_alloc&) {
        throw dataset_error("series out of bound");
    }
}

void RawSeries::copy(const RawSeries& src) {
    selectType(src.type());
    auto viter = src.begin();
    while(viter != src.end()) {
        if (viter.type() == ValueType::continous)
            insert(viter.value().getc(), nullpos);
        else
            insert(viter.value().getd().value(), nullpos);
        viter++;
    }
}
