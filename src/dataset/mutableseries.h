#ifndef DATASET_MUTABLESERIES_H
#define DATASET_MUTABLESERIES_H

#include "types.h"
#include "series.h"

namespace Vizzu
{
namespace Dataset
{

/**
 * @brief The MutableSeries class
 * @details This class is used to store a series of original values.
 *     It stores additianally the type of each value to perform the
 *     necessary conversations. 
 */
class MutableSeries :
    public AbstractSeries,
    public std::enable_shared_from_this<MutableSeries>
{
friend class Dataset;
friend class SeriesContainer;
public:
    int size() const override;
    SeriesId id() const override;
    const std::string& name() const override;
    ValueType type() const override;
	ValueType typeAt(int index) const override;
	Value valueAt(int index) const override;
    ValueIterator begin() const override;
    ValueIterator end() const override;

    void extend(int size);
    double rate(ValueType type) const;
	void select(ValueType type);
    void insert(double value, int position = nullpos);
    void insert(const char* value, int position = nullpos);
    void insert(ValueType vt, const Value& value, int position = nullpos);
    void modify(double value, int position);
    void modify(const char* value, int position);
    void modify(ValueType vt, const Value& value, int position);
    void erase(int index);

protected:
    Dataset& dataset;
    SeriesId seriesId;
    std::string seriesName;
    TypeVector vtypes;
    ValueVector values;
    ValueType selectedType;

    MutableSeries(Dataset& dataset);
    MutableSeries(Dataset& dataset, SeriesId id, const char* name);
};

}
}

#endif //DATASET_MUTABLESERIES_H
