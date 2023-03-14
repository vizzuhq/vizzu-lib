#ifndef DATASET_TABLE_H
#define DATASET_TABLE_H

#include "types.h"
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
public:
    Table(Dataset& dataset, const char* name);

    void setSorter(const TableSorterPtr& ptr);
    void setFilter(const TableFilterPtr& ptr);
    void setGenerator(const TableGeneratorPtr& ptr);
    void insertRow(int pos);
    void removeRow(int pos);
    void insertColumn(int pos, const ConstantSeriesPtr& ptr);
    void removeColumn(int pos, const ConstantSeriesPtr& ptr);

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
    TableId tableId;
    std::string tableName;
    TableSorterPtr sorter;
    TableFilterPtr filter;
    TableGeneratorPtr generator;
};

}
}

#endif //DATASET_TABLE_H
