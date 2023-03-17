#ifndef DATASET_TABLE_COLUMN_H
#define DATASET_TABLE_COLUMN_H

#include "types.h"
#include "tablecell.h"

namespace Vizzu
{
namespace Dataset
{

class Column {
friend class Cell;
friend class ConstantTable;
friend class ColumnContainer;
public:
    int position() const;
    Cell begin() const;
    Cell end() const;
    Column operator++(int) const;
    Column operator--(int) const;

protected:
    int colPosition;
    const AbstractConstantTable* table;

    Column(const AbstractConstantTable* table, int pos);
};

class ColumnContainer {
friend class ConstantTable;
public:
    int size() const;
    Column begin() const;
    Column end() const;

protected:
    const AbstractConstantTable* table;

    ColumnContainer(const AbstractConstantTable* table);
};

}
}

#endif //DATASET_TABLE_COLUMN_H
