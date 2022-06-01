#ifndef DATAFRAME_VALUE_H
#define DATAFRAME_VALUE_H

#include "types.h"

namespace Vizzu
{
namespace DataSet
{

class DiscreteValue {
friend class DiscreteValueContainer;
public:
    DiscreteValue(const DiscreteValue& arg);
    DiscreteValue(DiscreteValue&& arg);

    DiscreteId id() const;
    const char* value() const;

protected:
    DiscreteId discreteId;
    std::string discreteValue;

    DiscreteValue(DiscreteValueContainer *cont, const char* val);
};

class Value {
public:
    Value();
    Value(const Value& arg);
    Value(const DiscreteValue& dValue);
    Value(const ContinousValue& cVal);

    ValueId id();
    ValueType type();
    const DiscreteValue& getd();
    const ContinousValue& getc();

protected:
    union {
        double continous;
        const DiscreteValue* discrete;
    } value;
};

class ValueIterator {
public:
    const Value& operator*();
};

class DiscreteValueContainer {
public:
    DiscreteValueContainer(
        DataSet& dataSet);

    DiscreteValueContainer(
        DataSet& dataSet,
        DiscreteValueLookupFn dvalLookupFn,
        DiscreteValueEqTestFn dvalEqTest);

    const DiscreteValue& get(ValueId id);
    const DiscreteValue& get(DiscreteId id);
    const DiscreteValue& get(const char* dval);

protected:
    DataSet& dataSet;
    DiscreteValueLookupFn dvalLookupFn;
    DiscreteValueEqTestFn dvalEqTestFn;
    DiscreteValuesMap values;

    DiscreteId toDiscreteId(ValueId id);
    DiscreteId toDiscreteId(const char* dval);
};

}
}

#endif //DATAFRAME_VALUE_H
