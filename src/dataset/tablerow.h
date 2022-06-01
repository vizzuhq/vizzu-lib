#ifndef DATASET_TABLE_ROW_H
#define DATASET_TABLE_ROW_H

#include "types.h"
#include "series.h"
#include "value.h"
#include "dataset.h"

namespace Vizzu
{
namespace DataSet
{

class Row {
friend Table;
friend RowIterator;
public:
    const int pos;

    int size() const;
    CellIterator begin() const;
    CellIterator end() const;
    bool operator==(const Row& arg) const;
    bool operator>(const Row& arg) const;

protected:
    TablePtr table;
    Row(const TablePtr& table, int pos);
};

class RowIterator {
friend class RowContainer;
public:
    int pos() const;
    const Row& operator*();
    ValueIterator operator++(int) const;
    ValueIterator operator--(int) const;

protected:
    int position;
    const RowContainer& container;
    RowIterator(const RowContainer& cnt, int pos);
};

class RowContainer {
friend class Table;
public:
    int size() const;
    RowIterator begin() const;
    RowIterator end() const;

protected:
    TablePtr table;
    RowContainer(const TablePtr& table);
};

}
}

#endif //DATASET_TABLE_ROW_H
