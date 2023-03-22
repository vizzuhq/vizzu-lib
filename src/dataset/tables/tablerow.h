#ifndef DATASET_TABLE_ROW_H
#define DATASET_TABLE_ROW_H

#include "../types.h"
#include "tablecell.h"

namespace Vizzu
{
namespace Dataset
{

class Row {
friend class Cell;
friend class ConstantTable;
friend class RowIterator;
public:
    int position() const;
    CellIterator begin() const;
    CellIterator end() const;

protected:
    int rowPosition;
    const AbstractConstantTable* table;

    Row(const AbstractConstantTable* cnt, int pos);
};

class RowIterator {
friend class ConstantTable;
friend class RowContainer;
public:
    RowIterator operator++(int) const;
    RowIterator& operator++();
    RowIterator operator--(int) const;
    RowIterator& operator--();
    bool operator==(const RowIterator& arg) const;
    bool operator!=(const RowIterator& arg) const;
    Row operator*() const;

protected:
    int position;
    const AbstractConstantTable* table;

    RowIterator(const AbstractConstantTable* cnt, int pos);
};

class RowContainer {
friend class ConstantTable;
public:
    int size() const;
    RowIterator begin() const;
    RowIterator end() const;

protected:
    const AbstractConstantTable* table;

    RowContainer(const AbstractConstantTable* table);
};

}
}

#endif //DATASET_TABLE_ROW_H
