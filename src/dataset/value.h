#ifndef DATAFRAME_VALUE_H
#define DATAFRAME_VALUE_H

#include "types.h"

namespace Vizzu
{
namespace Dataset
{

/**
 * @brief DiscreteValue class
 *
 * @details This class represents a discrete value in the dataset.
 * It is a pair of an integer id and a textual value.
 * The id is the hash of the textual value.
 */
class DiscreteValue {
friend class DiscreteValueContainer;
public:
    DiscreteValue();
    DiscreteValue(const DiscreteValue& arg);
    DiscreteValue(DiscreteValue&& arg);

    DiscreteHash hash() const;
    const char* value() const;
    bool operator==(const DiscreteValue& arg) const;
    static DiscreteHash hash(const char*);
    DiscreteValue& operator=(DiscreteValue&& arg);
    DiscreteValue& operator=(const DiscreteValue& arg);

protected:
    static const uint32_t hashA = 54059;
    static const uint32_t hashB = 76963;
    static const uint32_t hashF = 37;

    DiscreteHash discreteHash;
    std::string discreteValue;

    DiscreteValue(const char* name);
    DiscreteValue(std::string&& name);
    DiscreteValue(DiscreteValue&& val, const char* name);
    void calculateHash();
};

/**
 * @brief Value class
 *
 * @details This class is used to represent a value in the data set.
 * It can be either a continous value or a discrete value.
 * It supports the following operations:
 * - get the hash based id of the value
 * - get the type of the value
 * - get the value as a continous value
 * - get the value as a discrete value
 * 
 */
class Value {
public:
    Value() noexcept;
    Value(const DiscreteValue& dVal) noexcept;
    Value(const ContinousValue& cVal) noexcept;

    const DiscreteValue& getd() const;
    const ContinousValue& getc() const;

protected:
    union {
        double continous;
        const DiscreteValue* discrete;
    } value;
};

/**
 * @brief The DiscreteValueContainer class
 *
 * @details This class is used to create and store unique discrete values.
 * It is used to store discrete values and id pairs in the Dataset.
 * It supports custom value lookup and equality test features.
 * 
 */
class DiscreteValueContainer {
public:
    int size() const;
    void clear();
    DiscreteValueSet::const_iterator begin() const;
    DiscreteValueSet::const_iterator end() const;
    const DiscreteValue& get(const char* value);
    void filter(const char* regexp, std::function<void(const DiscreteValue&)> fn) const;

protected:
    DiscreteValueSet values;
    DVNameSubstitutionFn nameSubstitutionFn;
};

}
}

#endif //DATAFRAME_VALUE_H
