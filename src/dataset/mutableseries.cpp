#include "dataset.h"
#include "value.h"
#include "valueiterator.h"
#include "mutableseries.h"

using namespace Vizzu::Dataset;

/**
 * Implementation of MutableSeries class
 */
MutableSeries::MutableSeries(Dataset& dataset)
    : dataset(dataset), seriesId(nullSeriesId), seriesName("")
{
}

MutableSeries::MutableSeries(Dataset& dataset, SeriesId id, const char* name)
    : dataset(dataset), seriesId(id), seriesName(name), selectedType(ValueType::null)
{
}

int MutableSeries::size() const {
    return values.size();
}

SeriesId MutableSeries::id() const {
    return seriesId;
}

const std::string& MutableSeries::name() const {
    return seriesName;
}

ValueType MutableSeries::type() const {
    return selectedType;
}

Value MutableSeries::valueAt(int index) const {
    try {
        return values[index];
    } catch (std::out_of_range&) {
        throw dataset_error("series index out of range");
    }
}

ValueType MutableSeries::typeAt(int position) const {
    try {
        if (selectedType == ValueType::null)
            return vtypes[position];
        return selectedType;
    } catch (std::out_of_range&) {
        throw dataset_error("series index out of range");
    }
}

ValueIterator MutableSeries::begin() const {
    const auto& ptr = shared_from_this();
    const auto& sptr = std::dynamic_pointer_cast<ConstSeriesPtr::element_type>(ptr);
    return ValueIterator{0, sptr};
}

ValueIterator MutableSeries::end() const {
    const auto& ptr = shared_from_this();
    const auto& sptr = std::dynamic_pointer_cast<ConstSeriesPtr::element_type>(ptr);
    return ValueIterator{size(), sptr};
}

double MutableSeries::rate(ValueType type) const {
    double count = 0;
    if (selectedType == ValueType::null) {
        for(auto& vt : vtypes) {
            if (vt == type)
                count++;
        }
    }
    else if (type == selectedType)
        count = values.size();
    return count / values.size();
}

void MutableSeries::select(ValueType type) {
    if (selectedType != ValueType::null && type == ValueType::null) {
        vtypes.reserve(values.size());
        for(int i = 0; i < (int)values.size(); i++)
            vtypes.push_back(selectedType);
        selectedType = ValueType::null;
    }
    else {
        for(int i = 0; i < (int)values.size(); i++) {
            if (typeAt(i) != type && type == ValueType::continous) {
                auto cval = dataset.D2CConverter(shared_from_this(), values[i].getd().value());
                values[i] = dataset.getValue(cval);
            }
            if (typeAt(i) != type && type == ValueType::discrete) {
                auto dval = dataset.C2DConverter(shared_from_this(), values[i].getc());
                values[i] = dataset.getValue(dval.c_str());
            }    
        }
        vtypes.clear();
        vtypes.shrink_to_fit();
        selectedType = type;
    }
}

void MutableSeries::extend(int size) {
    try {
        values.reserve(size);
        if (selectedType == ValueType::null)
            vtypes.reserve(size);
    } catch (std::bad_alloc&) {
        throw dataset_error("series out of memory");
    }
}

void MutableSeries::insert(double value, int pos) {
    insert(ValueType::continous, dataset.getValue(value), pos);
}

void MutableSeries::insert(const char* value, int pos) {
    insert(ValueType::discrete, dataset.getValue(value), pos);
}

void MutableSeries::insert(ValueType vt, const Value& value, int pos) {
    if (vt == ValueType::null)
        throw dataset_error("null value insertion");
    if (pos == nullpos)
        pos = values.size();
    auto type = selectedType;
    if (type == ValueType::null) {
        type = vt;
        vtypes.insert(vtypes.begin() + pos, vt);
    }
    if (vt == ValueType::discrete) {
        if (type == ValueType::discrete)
            values.insert(values.begin() + pos, value);
        else {
            auto conv = dataset.D2CConverter(shared_from_this(), value.getd().value());
            values.insert(values.begin() + pos, dataset.getValue(conv));
        }
    }
    else {
        if (type == ValueType::continous)
            values.insert(values.begin() + pos, value);
        else {
            auto conv = dataset.C2DConverter(shared_from_this(), value.getc());
            values.insert(values.begin() + pos, dataset.getValue(conv.c_str()));
        }
    }
}

void MutableSeries::modify(double value, int pos) {
    modify(ValueType::continous, dataset.getValue(value), pos);
}

void MutableSeries::modify(const char* value, int pos) {
    modify(ValueType::discrete, dataset.getValue(value), pos);
}

void MutableSeries::modify(ValueType vt, const Value& value, int pos) {
    try {
        if (selectedType == ValueType::null)
            vtypes[pos] = vt;
        if (selectedType == ValueType::discrete && vt == ValueType::continous) {
            auto conv = dataset.D2CConverter(shared_from_this(), value.getd().value());
            values[pos] = dataset.getValue(conv);
        }
        else if (selectedType == ValueType::continous && vt == ValueType::discrete) {
            auto conv = dataset.C2DConverter(shared_from_this(), value.getc());
            values[pos] = dataset.getValue(conv.c_str());
        }
        else
            values[pos] = value;
    } catch (std::bad_alloc&) {
        throw dataset_error("series out of bound");
    }
}

void MutableSeries::erase(int index) {
    try {
        values.erase(values.begin() + index);
        if (selectedType == ValueType::null)
            vtypes.erase(vtypes.begin() + index);
    } catch (std::bad_alloc&) {
        throw dataset_error("series out of bound");
    }
}
