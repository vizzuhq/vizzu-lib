#include "types.h"
#include "value.h"

using namespace Vizzu::DataSet;

/**
 * Implementation of DiscreteValue class
 */
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

DiscreteHash DiscreteValue::hash() const {
    return discreteHash;
}

const char* DiscreteValue::value() const {
    return discreteValue.c_str();
}

void DiscreteValue::calculateHash() {
    discreteHash = hashF;
    for(auto c : discreteValue)
        discreteHash = (discreteHash * hashA) ^ (c * hashB);
}

/**
 * Implementation of Value class
 */
Value::Value(const Value& arg) {
    value = arg.value;
}

Value::Value(const DiscreteValue& dValue) {
    value.discrete = &dValue;
}

Value::Value(const ContinousValue& cVal) {
    value.continous = cVal;
}

const DiscreteValue& Value::getd() {
    return *value.discrete;
}

const ContinousValue& Value::getc() {
    return value.continous;
}

/**
 * Implementation of DiscreteValueContainer class
 */
const DiscreteValue& DiscreteValueContainer::get(const char* dval) {
    std::string name;
    if (nameSubstitutionFn)
        name = nameSubstitutionFn(dval);
    else
        name = dval;
    auto it = values.find(name.c_str());
    if (it == values.end()) {
        DiscreteValue value(std::move(name));
        auto result = values.insert(std::make_pair(name.c_str(), value));
        return result.first->second;
    }
    return it->second;
}
