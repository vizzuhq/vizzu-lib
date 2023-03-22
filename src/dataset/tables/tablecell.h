#ifndef DATASET_TABLE_CELL_H
#define DATASET_TABLE_CELL_H

#include "../types.h"
#include "../series/value.h"
#include "../series/iterators.h"

namespace Vizzu
{
namespace Dataset
{

class Cell {
friend class ConstantTable;
friend class CellIterator;
public:
    int rowPos() const;
    int colPos() const;
    Value value() const;
    ValueType type() const;
    DatasetId id() const;

protected:
    int row;
    int column;
    const AbstractConstantTable* table;

    Cell(const AbstractConstantTable* table, int row, int col);
};

class CellIterator {
friend class Row;
friend class Column;
friend class ConstantTable;
public:
    CellIterator operator++(int) const;
    CellIterator& operator++();
    CellIterator operator--(int) const;
    CellIterator& operator--();
    bool operator==(const CellIterator& arg) const;
    bool operator!=(const CellIterator& arg) const;
    Cell operator*() const;

protected:
    int row;
    int column;
    bool direction;
    const AbstractConstantTable* table;

    CellIterator(const AbstractConstantTable* table, bool dir, int col, int row);
};

}
}

#endif //DATASET_TABLE_CELL_H
