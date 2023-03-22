#include "../dataset.h"
#include "../series/value.h"
#include "../series/iterators.h"
#include "../series/rawseries.h"
#include "aggregators.h"

namespace Vizzu {
namespace Dataset {
namespace Aggregators {

void Count::setup(const Dataset&) {
    count = 0;
}

ValueType Count::type() {
    return ValueType::continous;
}

void Count::aggregateRecord(int) {
    count++;
}

Value Count::aggregatedValue() {
    int result = count;
    count = 0;
    return Value{result};
}

Min::Min(const char* seriesName)
    : firstValue(true), minimum(0), seriesName(seriesName)
{
}

void Min::setup(const Dataset& ds) {
    series = ds.getSeries(seriesName.c_str());
    if (!series || series->type() != ValueType::continous)
        throw dataset_error("continous data series required");
    minimum = 0;
    firstValue = true;
}

ValueType Min::type() {
    return ValueType::continous;
}

void Min::aggregateRecord(int index) {
    auto value = series->valueAt(index).getc();
    if (firstValue)
        minimum = value;
    else if (value < minimum)
        minimum = value;
    firstValue = false;
}

Value Min::aggregatedValue() {
    firstValue = true;
    return Value{minimum};
}

Max::Max(const char* seriesName)
    : firstValue(true), maximum(0), seriesName(seriesName)
{
}

void Max::setup(const Dataset& ds) {
    series = ds.getSeries(seriesName.c_str());
    if (!series || series->type() != ValueType::continous)
        throw dataset_error("continous data series required");
    maximum = 0;
    firstValue = true;
}

ValueType Max::type() {
    return ValueType::continous;
}

void Max::aggregateRecord(int index) {
    auto value = series->valueAt(index).getc();
    if (firstValue)
        maximum = value;
    else if (value > maximum)
        maximum = value;
    firstValue = false;
}

Value Max::aggregatedValue() {
    firstValue = true;
    return Value{maximum};
}

Avarage::Avarage(const char* seriesName)
    : firstValue(true), avarage(0), seriesName(seriesName)
{
}

void Avarage::setup(const Dataset& ds) {
    series = ds.getSeries(seriesName.c_str());
    if (!series || series->type() != ValueType::continous)
        throw dataset_error("continous data series required");
    avarage = 0;
    firstValue = true;
}

ValueType Avarage::type() {
    return ValueType::continous;
}

void Avarage::aggregateRecord(int index) {
    auto value = series->valueAt(index).getc();
    if (firstValue)
        avarage = value;
    else
        avarage = (avarage + value) / 2;
    firstValue = false;
}

Value Avarage::aggregatedValue() {
    firstValue = true;
    return Value{avarage};
}

}
}
}
