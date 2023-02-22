#ifndef DATASET_DATASET_H
#define DATASET_DATASET_H

#include "types.h"
#include "series.h"
#include "value.h"
#include "table.h"

namespace Vizzu
{
namespace DataSet
{

class DataSet {
public:
    DataSet();

    Value getValue(double continousValue);
    Value getValue(const char* discreteValue);
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
