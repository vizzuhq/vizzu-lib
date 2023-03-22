#ifndef DATASET_TABLE_COLUMN_H
#define DATASET_TABLE_COLUMN_H

#include "../types.h"
#include "tablecell.h"

namespace Vizzu
{
namespace Dataset
{

class Column {
friend class Cell;
friend class ConstantTable;
friend class ColumnIterator;
public:
    int position() const;
    CellIterator begin() const;
    CellIterator end() const;

protected:
    int colPosition;
    const AbstractConstantTable* table;

    Column(const AbstractConstantTable* table, int pos);
};

class ColumnIterator {
friend class ConstantTable;
friend class ColumnContainer;
public:
    ColumnIterator operator++(int) const;
    ColumnIterator& operator++();
    ColumnIterator operator--(int) const;
    ColumnIterator& operator--();
    bool operator==(const ColumnIterator& arg) const;
    bool operator!=(const ColumnIterator& arg) const;
    Column operator*() const;

protected:
    int position;
    const AbstractConstantTable* table;

    ColumnIterator(const AbstractConstantTable* table, int pos);
};

class ColumnContainer {
friend class ConstantTable;
public:
    int size() const;
    ColumnIterator begin() const;
    ColumnIterator end() const;

protected:
    const AbstractConstantTable* table;

    ColumnContainer(const AbstractConstantTable* table);
};

}
}

#endif //DATASET_TABLE_COLUMN_H
