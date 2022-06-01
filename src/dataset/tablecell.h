#ifndef DATASET_TABLE_CELL_H
#define DATASET_TABLE_CELL_H

#include "types.h"
#include "series.h"
#include "value.h"
#include "dataset.h"

namespace Vizzu
{
namespace DataSet
{

class Cell {
friend RowIterator;
friend ColIterator;
public:
    const int rowPos;
    const int colPos;

    const Value& operator*();
    Cell& operator=(const Value&);
    const SeriesInfo& info();
    bool operator==(const Cell& arg) const;
    bool operator>(const Cell& arg) const;

protected:
    TablePtr table;
    ValueIterator value;
};

class CellIterator {
friend class Row;
public:
    int row() const;
    int col() const;
    void nextCol();
    void prevCol();
    void nextRow();
    void prevRow();
    const Cell& operator*();

protected:
    int colPosition;
    int rowPosition;
    TablePtr table;
    CellIterator(const TablePtr& table, int col, int row);
};

}
}

#endif //DATASET_TABLE_CELL_H
