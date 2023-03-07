#ifndef DATASET_DATASET_H
#define DATASET_DATASET_H

#include "types.h"
#include "series.h"
#include "value.h"
#include "table.h"
#include "range.h"
#include "mutableseries.h"
#include "iterators.h"

namespace Vizzu
{
namespace Dataset
{

class Dataset {
public:
    DiscreteToContinousConverterFn D2CConverter;
    ContinousToDiscreteConverterFn C2DConverter;

    Dataset();
    Dataset(const Dataset& src);

    void clear();
    bool empty();
    void compact();
    void deepCopy(const Dataset& src);
    Value getValue(double continousValue);
    Value getValue(const char* discreteValue);
    const DiscreteValueContainer& discreteValues() const;
    const SeriesContainer& mutableSeries() const;
    MutableSeriesPtr getMutableSeries(const char* name);
    MutableSeriesPtr addMutableSeries(const char* name);
    TablePtr addTable(const char* name);
    TablePtr addTable(const char* name, TableBuilderPtr builder);

protected:
    TableContainer tables;
    SeriesContainer series;
    DiscreteValueContainer values;
};

}
}

#endif //DATASET_DATASET_H
