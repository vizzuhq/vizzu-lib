#include "dataset.h"
#include "value.h"
#include "iterators.h"
#include "originalseries.h"
#include "range.h"

using namespace Vizzu::Dataset;

OriginalSeries::OriginalSeries(Dataset& dataset)
    : BaseSeries(dataset)
{
}

OriginalSeries::OriginalSeries(Dataset& dataset, DatasetId id, const char* name)
    : BaseSeries(dataset, id, name)
{
}

OriginalSeries::OriginalSeries(const OriginalSeries& src)
    : BaseSeries(src.dataset, src.seriesId, src.seriesName.c_str())
{
    copy(src);
}

OriginalSeries::OriginalSeries(Dataset& dataset, DatasetId id, const char* name, const OriginalSeries& src)
    : BaseSeries(dataset, id, name)
{
    copy(src);
}

Dataset& OriginalSeries::owner() const {
    return dataset;
}

int OriginalSeries::size() const {
    return values.size();
}

DatasetId OriginalSeries::id() const {
    return seriesId;
}

const char* OriginalSeries::name() const {
    return seriesName.c_str();
}

ValueType OriginalSeries::type() const {
    return seriesType;
}

Value OriginalSeries::valueAt(int index) const {
    try {
        return values[index];
    } catch (std::out_of_range&) {
        throw dataset_error("series index out of range");
    }
}

ValueType OriginalSeries::typeAt(int position) const {
    try {
        if (seriesType == ValueType::null)
            return vtypes[position];
        return seriesType;
    } catch (std::out_of_range&) {
        throw dataset_error("series index out of range");
    }
}

ValueIterator OriginalSeries::begin() const {
    return ValueIterator{0, this};
}

ValueIterator OriginalSeries::end() const {
    return ValueIterator{size(), this};
}

double OriginalSeries::typeRate(ValueType type) const {
    return calculateTypeRate(*this, type);
}

void OriginalSeries::selectType(ValueType type) {
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

void OriginalSeries::extend(int size) {
    try {
        values.reserve(size);
        if (seriesType == ValueType::null)
            vtypes.reserve(size);
    } catch (std::bad_alloc&) {
        throw dataset_error("series out of memory");
    }
}

void OriginalSeries::insert(double value, int pos) {
    insert(ValueType::continous, dataset.getValue(value), pos);
}

void OriginalSeries::insert(const char* value, int pos) {
    insert(ValueType::discrete, dataset.getValue(value), pos);
}

void OriginalSeries::insert(ValueType vt, const Value& value, int pos) {
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

void OriginalSeries::modify(double value, int pos) {
    modify(ValueType::continous, dataset.getValue(value), pos);
}

void OriginalSeries::modify(const char* value, int pos) {
    modify(ValueType::discrete, dataset.getValue(value), pos);
}

void OriginalSeries::modify(ValueType vt, const Value& value, int pos) {
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

void OriginalSeries::erase(int index) {
    try {
        values.erase(values.begin() + index);
        if (seriesType == ValueType::null)
            vtypes.erase(vtypes.begin() + index);
    } catch (std::bad_alloc&) {
        throw dataset_error("series out of bound");
    }
}

void OriginalSeries::copy(const OriginalSeries& src) {
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
