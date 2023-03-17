#ifndef DATASET_TABLE_CELL_H
#define DATASET_TABLE_CELL_H

#include "types.h"
#include "value.h"
#include "iterators.h"

namespace Vizzu
{
namespace Dataset
{

class Cell {
friend class ConstantTable;
public:
    Row row() const;
    Column col() const;
    void nextCol();
    void prevCol();
    void nextRow();
    void prevRow();
    const Value& operator*();

protected:
    int colPosition;
    int rowPosition;
    const AbstractConstantTable* table;

    Cell(const AbstractConstantTable* table, int col, int row);
};

}
}

#endif //DATASET_TABLE_CELL_H
