#ifndef DATASET_LINKED_H
#define DATASET_LINKED_H

#include "types.h"
#include "interfaces.h"
#include "baseseries.h"

namespace Vizzu {
namespace Dataset {

class LinkedSeries :
    public BaseSeries,
	public AbstractConstantSeries,
    public std::enable_shared_from_this<LinkedSeries>
{
public:
	using selection_vector = std::vector<int>;
	using selection_vector_ptr = std::shared_ptr<selection_vector>;

public:
	LinkedSeries(const ConstantSeriesPtr& series, DatasetId id, const char* name);

	int size() const override;
	DatasetId id() const override;
	const char* name() const override;
	ValueType type() const override;
	ValueType typeAt(int index) const override;
	Value valueAt(int index) const override;
	ValueIterator begin() const override;
	ValueIterator end() const override;

	void createSelector(int size);
	void setSelector(const selection_vector_ptr& sel);
	selection_vector_ptr getSelector() const;
	void releaseSelector();
	void select(int recordIndex);
	void deselect(int recordIndex);

protected:
	ConstantSeriesPtr series;
	selection_vector_ptr selector;
};

}
}

#endif // DATASET_LINKED_H
