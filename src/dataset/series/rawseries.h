#ifndef DATASET_MUTABLESERIES_H
#define DATASET_MUTABLESERIES_H

#include "../types.h"
#include "baseseries.h"

namespace Vizzu
{
namespace Dataset
{

class RawSeries :
    public BaseSeries,
    public AbstractConstantSeries,
	public AbstractVolatileSeries,
    public std::enable_shared_from_this<RawSeries>
{
public:
    RawSeries(Dataset& dataset);
    RawSeries(Dataset& dataset, const char* name);
    RawSeries(const RawSeries& src);
    RawSeries(Dataset& dataset, const char* name, const RawSeries& src);

    double typeRate(ValueType type) const;
	void selectType(ValueType type);
    void selectType(ValueType type, ValueConverter conv);

public:
    Dataset& owner() const override;
	int size() const override;
	DatasetId id() const override;
	const char* name() const override;
    const char* getParam(const char* name) const override;
    void addParam(const char* name, const char* value) override;
	ValueType type() const override;
	ValueType typeAt(int index) const override;
	Value valueAt(int index) const override;
	ValueIterator begin() const override;
	ValueIterator end() const override;

public:
    void extend(int size) override;
    void insert(double value, int position = nullpos) override;
    void insert(std::span<double> values, int position = nullpos) override;
    void insert(const char* value, int position = nullpos) override;
    void insert(ValueType vt, const Value& value, int position = nullpos) override;
    void modify(double value, int position) override;
    void modify(const char* value, int position) override;
    void modify(ValueType vt, const Value& value, int position) override;
    void erase(int index) override;

protected:
    TypeVector vtypes;
    ValueVector values;

    void copy(const RawSeries& src);
};

}
}

#endif //DATASET_MUTABLESERIES_H
