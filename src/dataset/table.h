#ifndef DATASET_TABLE_H
#define DATASET_TABLE_H

#include "types.h"
#include "series.h"
#include "value.h"
#include "dataset.h"

namespace Vizzu
{
namespace DataSet
{

class Table {
public:
    Table(DataSet& dataset);

    void setSorter(const SorterPtr& ptr);
    void setFilter(const FilterPtr& ptr);
    void insertRow(int pos);
    void removeRow(int pos);
    void insertColumn(int pos, SeriesPtr ptr);
    void removeColumn(int pos, SeriesPtr ptr);

    RowContainer rows() const;
    ColContainer cols() const;
    Row row(int pos) const;
    Column col(int pos) const;
    Cell at(int col, int row) const;

protected:
    SorterPtr sorter;
    FilterPtr filter;
    TableBuilderPtr builder;
    SeriesIndexPtr index;
    SeriesVector series;
};

class TableContainer {
public:
    TableContainer();

    int size();
    TableIterator begin();
    TableIterator end();

protected:
    TableMap tables;
};

}
}

#endif //DATASET_TABLE_H
