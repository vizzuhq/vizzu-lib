#ifndef DATASET_TABLE_H
#define DATASET_TABLE_H

#include "types.h"
#include "series.h"
#include "interfaces.h"

namespace Vizzu
{
namespace Dataset
{

class Table :
    public std::enable_shared_from_this<Table>
{
friend class RowContainer;
friend class ColumnContainer;
friend class TableContainer;
public:
    Table(Dataset& dataset, const char* name);

    void setSorter(const TableSorterPtr& ptr);
    void setFilter(const TableFilterPtr& ptr);
    void setGenerator(const TableGeneratorPtr& ptr);
    void insertRow(int pos);
    void removeRow(int pos);
    void insertColumn(int pos, SeriesPtr ptr);
    void removeColumn(int pos, SeriesPtr ptr);

    RowContainer rows() const;
    ColumnContainer cols() const;
    Row row(int pos) const;
    Column col(int pos) const;
    Cell cell(int col, int row) const;

    template<class T>
    void setSorter(const T& ptr) {
        setSorter(std::dynamic_pointer_cast<TableSorterPtr::element_type>(ptr));
    }
    template<class T>
    void setFilter(const T& ptr) {
        setFilter(std::dynamic_pointer_cast<TableFilterPtr::element_type>(ptr));
    }
    template<class T>
    void setGenerator(const T& ptr) {
        setGenerator(std::dynamic_pointer_cast<TableGeneratorPtr::element_type>(ptr));
    }

protected:
    Dataset& dataset;
    TableId id;
    std::string name;
    TableSorterPtr sorter;
    TableFilterPtr filter;
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
