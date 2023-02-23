#include "dataset.h"
#include "value.h"
#include "valueiterator.h"
#include "mutableseries.h"

using namespace Vizzu::DataSet;

/**
 * Implementation of MutableSeries class
 */
MutableSeries::MutableSeries(DataSet& dataset)
    : dataset(dataset), seriesId(nullSeriesId), seriesName("")
{
}

MutableSeries::MutableSeries(DataSet& dataset, SeriesId id, const char* name)
    : dataset(dataset), seriesId(id), seriesName(name), normlizedType(ValueType::null)
{
    values.reserve(128);
    vtypes.reserve(128);
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
    return ValueType::null;
}

const Value& MutableSeries::at(int index) const {
    try {
        return values[index];
    } catch (std::out_of_range&) {
        throw dataset_error("series index out of range");
    }
    catch (...) {
        throw dataset_error("unknown series error");
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

ValueType MutableSeries::typeAt(int position) const {
    try {
        return vtypes[position];
    } catch (std::out_of_range&) {
        throw dataset_error("series index out of range");
    }
    catch (...) {
        throw dataset_error("unknown series error");
    }
}

double MutableSeries::typeRate(ValueType type) const {
    double count = 0;
    for(auto& vt : vtypes) {
        if (vt == type)
            count++;
    }
    return count / vtypes.size();
}

void MutableSeries::normalize(ValueType type) {
    for(int i = 0; i < (int)vtypes.size(); i++) {
        if (vtypes[i] != type && type == ValueType::continous) {
            auto cval = dataset.D2CNormalizer(shared_from_this(), values[i].getd().value());
            values[i] = dataset.getValue(cval);
        }
        if (vtypes[i] != type && type == ValueType::discrete) {
            auto dval = dataset.C2DNormalizer(shared_from_this(), values[i].getc());
            values[i] = dataset.getValue(dval.c_str());
        }    
    }
}

void MutableSeries::extend(int size) {
    try {
        values.reserve(size);
        vtypes.reserve(size);
    } catch (std::bad_alloc&) {
        throw dataset_error("series out of memory");
    }
    catch (...) {
        throw dataset_error("unknown series error");
    }
}

void MutableSeries::insert(double value, int pos) {
    if (pos == nullpos)
        pos = values.size();
    try {
        values.insert(values.begin() + pos, dataset.getValue(value));
        vtypes.insert(vtypes.begin() + pos, ValueType::continous);
    } catch (std::bad_alloc&) {
        throw dataset_error("series out of memory");
    }
    catch (...) {
        throw dataset_error("unknown series error");
    }
}

void MutableSeries::insert(const char* value, int pos) {
    if (pos == nullpos)
        pos = values.size();
    try {
        values.insert(values.begin() + pos, dataset.getValue(value));
        vtypes.insert(vtypes.begin() + pos, ValueType::discrete);
    } catch (std::bad_alloc&) {
        throw dataset_error("series out of memory");
    }
    catch (...) {
        throw dataset_error("unknown series error");
    }
}

void MutableSeries::insert(ValueType vt, const Value& value, int pos) {
    if (pos == nullpos)
        pos = values.size();
    values.insert(values.begin() + pos, value);
    vtypes.insert(vtypes.begin() + pos, vt);
}

void MutableSeries::modify(double value, int position) {
    try {
        values[position] = dataset.getValue(value);
        vtypes[position] = ValueType::continous;
    } catch (std::bad_alloc&) {
        throw dataset_error("series out of bound");
    }
    catch (...) {
        throw dataset_error("unknown series error");
    }
}

void MutableSeries::modify(const char* value, int position) {
    try {
        values[position] = dataset.getValue(value);
        vtypes[position] = ValueType::discrete;
    } catch (std::bad_alloc&) {
        throw dataset_error("series out of bound");
    }
    catch (...) {
        throw dataset_error("unknown series error");
    }
}

void MutableSeries::modify(ValueType vt, const Value& value, int position) {
    try {
        values[position] = value;
        vtypes[position] = vt;
    } catch (std::bad_alloc&) {
        throw dataset_error("series out of bound");
    }
    catch (...) {
        throw dataset_error("unknown series error");
    }
}

void MutableSeries::erase(int index) {
    try {
        values.erase(values.begin() + index);
        vtypes.erase(vtypes.begin() + index);
    } catch (std::bad_alloc&) {
        throw dataset_error("series out of bound");
    }
    catch (...) {
        throw dataset_error("unknown series error");
    }
}
