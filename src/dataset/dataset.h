#ifndef DATASET_DATASET_H
#define DATASET_DATASET_H

#include "types.h"
#include "series.h"
#include "value.h"
#include "table.h"
#include "mutableseries.h"
#include "valueiterator.h"

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
    const DiscreteValueContainer& values() const;
    const SeriesContainer& mutableSeries() const;
    MutableSeriesPtr getMutableSeries(const char* name);
    MutableSeriesPtr makeMutableSeries(const char* name);

protected:
    TableContainer tables;
    SeriesContainer series;
    DiscreteValueContainer discreteValues;
};

}
}

#endif //DATASET_DATASET_H
