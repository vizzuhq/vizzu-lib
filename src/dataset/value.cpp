#include "types.h"
#include "value.h"

using namespace Vizzu::Dataset;

/**
 * Implementation of DiscreteValue class
 */
DiscreteValue::DiscreteValue() {
    discreteHash = 0;
}

DiscreteValue::DiscreteValue(const DiscreteValue& arg) {
    discreteHash = arg.discreteHash;
    discreteValue = arg.discreteValue;
}

DiscreteValue::DiscreteValue(DiscreteValue&& arg) {
    discreteHash = arg.discreteHash;
    discreteValue = std::move(arg.discreteValue);
}

DiscreteValue::DiscreteValue(std::string&& name) {
    discreteValue = std::move(name);
    calculateHash();
}

DiscreteValue::DiscreteValue(const char* name) {
    discreteHash = hash(name);
}

DiscreteValue::DiscreteValue(DiscreteValue&& val, const char* name) {
    discreteHash = val.discreteHash;
    if (val.discreteValue.empty())
        discreteValue = name;
    else
        discreteValue = std::move(val.discreteValue);
}

DiscreteHash DiscreteValue::hash() const {
    return discreteHash;
}

const char* DiscreteValue::value() const {
    return discreteValue.c_str();
}

bool DiscreteValue::operator==(const DiscreteValue& arg) const {
    return discreteHash == arg.discreteHash;
}

uint64_t DiscreteValue::hash(const char* str) {
    int pos = 0;
    uint64_t hash = hashF;
    while(str[pos])
        hash = (hash * hashA) ^ (str[pos++] * hashB);
    return hash;
}

DiscreteValue& DiscreteValue::operator=(DiscreteValue&& arg) {
    discreteHash = arg.discreteHash;
    if (!arg.discreteValue.empty())
        discreteValue = std::move(arg.discreteValue);
    return *this;
}

DiscreteValue& DiscreteValue::operator=(const DiscreteValue& arg) {
    discreteHash = arg.discreteHash;
    discreteValue = arg.discreteValue;
    return *this;
}

void DiscreteValue::calculateHash() {
    discreteHash = (uint32_t)hash(discreteValue.c_str());
}

/**
 * Implementation of Value class
 */
Value::Value() noexcept {
    value.continous = 0.0;
}

Value::Value(const DiscreteValue& dValue) noexcept {
    value.discrete = &dValue;
}

Value::Value(const ContinousValue& cVal) noexcept {
    value.continous = cVal;
}

const DiscreteValue& Value::getd() const {
    return *value.discrete;
}

const ContinousValue& Value::getc() const {
    return value.continous;
}

/**
 * Implementation of DiscreteValueContainer class
 */
int DiscreteValueContainer::size() const {
    return values.size();
}

void DiscreteValueContainer::clear() {
}

DiscreteValueSet::const_iterator DiscreteValueContainer::begin() const {
    return values.cbegin();
}

DiscreteValueSet::const_iterator DiscreteValueContainer::end() const {
    return values.cend();
}

const DiscreteValue& DiscreteValueContainer::get(const char* name) {
    DiscreteValue dval;
    if (nameSubstitutionFn) {
        dval = DiscreteValue{nameSubstitutionFn(name)};
    }
    else
        dval = DiscreteValue{name};
    auto it = values.find(dval);
    if (it == values.end()) {
        auto result = values.insert(DiscreteValue {std::move(dval), name});
        return *result.first;
    }
    return *it;
}

void DiscreteValueContainer::filter(const char*, std::function<void(const DiscreteValue&)> fn) const {
    for(const auto& dval : values)
        fn(dval);
}