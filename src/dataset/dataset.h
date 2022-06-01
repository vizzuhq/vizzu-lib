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

    const Value& getValue(double cval);
    const Value& getValue(const char* dval);
    SeriesId addSeries(const ValueVector& values);
    RecordId addRecord(const ValueVector& values);

protected:
    SeriesContainer series;
    TableContainer tables;
    DiscreteValueContainer dictionary;
};

}
}

#endif //DATASET_DATASET_H
