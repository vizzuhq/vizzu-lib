#ifndef DATASET_TABLE_H
#define DATASET_TABLE_H

#include "types.h"
#include "series.h"

namespace Vizzu
{
namespace Dataset
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
friend class TableContainer;
public:
    Table(Dataset& dataset, const char* name);
    Table(Dataset& dataset, const char* name, const TableGeneratorPtr& generator);

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
    Dataset& dataset;
    TableId id;
    std::string name;
    SorterPtr sorter;
    FilterPtr filter;
    TableGeneratorPtr generator;
    SeriesIndexPtr index;
    SeriesVector series;
};

class TableContainer {
friend class Dataset;
public:
    TableContainer();

    int size();
    void clear();
    TableIterator begin();
    TableIterator end();

protected:
    static TableId nextId;
    TableMap tables;

    void insertTable(const TablePtr& table);
};

}
}

#endif //DATASET_TABLE_H
