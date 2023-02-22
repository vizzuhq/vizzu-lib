#ifndef DATASET_TABLE_H
#define DATASET_TABLE_H

#include "types.h"
#include "series.h"

namespace Vizzu
{
namespace DataSet
{

class AbstractSorter {
};

class AbstractFilter {
};

class TableBuilder {
};

class Table :
    public std::enable_shared_from_this<Table>
{
friend class RowContainer;
friend class ColumnContainer;
public:
    Table(DataSet& dataset);

    void setSorter(const SorterPtr& ptr);
    void setFilter(const FilterPtr& ptr);
    void insertRow(int pos);
    void removeRow(int pos);
    void insertColumn(int pos, SeriesPtr ptr);
    void removeColumn(int pos, SeriesPtr ptr);

    RowContainer rows() const;
    ColumnContainer cols() const;
    Row row(int pos) const;
    Column col(int pos) const;
    Cell cell(int col, int row) const;

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
