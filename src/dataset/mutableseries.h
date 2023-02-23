#ifndef DATASET_MUTABLESERIES_H
#define DATASET_MUTABLESERIES_H

#include "types.h"
#include "series.h"

namespace Vizzu
{
namespace DataSet
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
friend class DataSet;
friend class SeriesContainer;
public:
    int size() const override;
    SeriesId id() const override;
    const std::string& name() const override;
    ValueType type() const override;
    const Value& at(int position) const override;
    ValueIterator begin() const override;
    ValueIterator end() const override;

    void extend(int size);
    ValueType typeAt(int position) const;
    double typeRate(ValueType type) const;
	void normalize(ValueType type);
    void insert(double value, int position = nullpos);
    void insert(const char* value, int position = nullpos);
    void insert(ValueType vt, const Value& value, int position = nullpos);
    void modify(double value, int position);
    void modify(const char* value, int position);
    void modify(ValueType vt, const Value& value, int position);
    void erase(int index);

protected:
    DataSet& dataset;
    SeriesId seriesId;
    std::string seriesName;
    TypeVector vtypes;
    ValueVector values;
    ValueType normlizedType;

    MutableSeries(DataSet& dataset);
    MutableSeries(DataSet& dataset, SeriesId id, const char* name);
};

}
}

#endif //DATASET_MUTABLESERIES_H
