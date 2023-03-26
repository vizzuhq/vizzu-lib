#ifndef DATASET_LINKED_H
#define DATASET_LINKED_H

#include "../types.h"
#include "../interface.h"
#include "baseseries.h"

namespace Vizzu {
namespace Dataset {

class LinkedSeries :
    public BaseSeries,
	public AbstractConstantSeries,
    public std::enable_shared_from_this<LinkedSeries>
{
public:
	using indices = std::vector<int>;
	using indices_ptr = std::shared_ptr<indices>;

public:
	LinkedSeries(const ConstantSeriesPtr& series);
	LinkedSeries(const ConstantSeriesPtr& series, const char* name);

	int size() const override;
	Dataset& owner() const override;
	DatasetId id() const override;
	const char* name() const override;
	const char* getParam(const char* name) const override;
	void addParam(const char* name, const char* value) override;
	ValueType type() const override;
	ValueType typeAt(int index) const override;
	Value valueAt(int index) const override;
	ValueIterator begin() const override;
	ValueIterator end() const override;	

	int originalSize() const;
	const ConstantSeriesPtr& originalSeries();
	indices_ptr createSelector(int size);
	void setSelector(const indices_ptr& sel);
	indices_ptr getSelector() const;
	void releaseSelector();
	void select(int recordIndex);
	void deselect(int recordIndex);

protected:
	ConstantSeriesPtr series;
	indices_ptr selector;
};

using LinkedSeriesPtr = std::shared_ptr<LinkedSeries>;

}
}

#endif // DATASET_LINKED_H
