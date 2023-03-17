#ifndef DATASET_MUTABLESERIES_H
#define DATASET_MUTABLESERIES_H

#include "types.h"
#include "baseseries.h"

namespace Vizzu
{
namespace Dataset
{

class OriginalSeries :
    public BaseSeries,
    public AbstractConstantSeries,
	public AbstractVolatileSeries,
    public std::enable_shared_from_this<OriginalSeries>
{
public:
    OriginalSeries(Dataset& dataset);
    OriginalSeries(Dataset& dataset, DatasetId id, const char* name);
    OriginalSeries(const OriginalSeries& src);
    OriginalSeries(Dataset& dataset, DatasetId id, const char* name, const OriginalSeries& src);

    double typeRate(ValueType type) const;
	void selectType(ValueType type);

public:
    Dataset& owner() const override;
	int size() const override;
	DatasetId id() const override;
	const char* name() const override;
	ValueType type() const override;
	ValueType typeAt(int index) const override;
	Value valueAt(int index) const override;
	ValueIterator begin() const override;
	ValueIterator end() const override;

public:
    void extend(int size) override;
    void insert(double value, int position = nullpos) override;
    void insert(const char* value, int position = nullpos) override;
    void insert(ValueType vt, const Value& value, int position = nullpos) override;
    void modify(double value, int position) override;
    void modify(const char* value, int position) override;
    void modify(ValueType vt, const Value& value, int position) override;
    void erase(int index) override;

protected:
    TypeVector vtypes;
    ValueVector values;

    void copy(const OriginalSeries& src);
};

}
}

#endif //DATASET_MUTABLESERIES_H
