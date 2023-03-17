#ifndef DATASET_TABLE_ROW_H
#define DATASET_TABLE_ROW_H

#include "types.h"
#include "tablecell.h"

namespace Vizzu
{
namespace Dataset
{

class Row {
friend class Cell;
friend class ConstantTable;
friend class RowContainer;
public:
    int position() const;
    Cell begin() const;
    Cell end() const;
    Row operator++(int) const;
    Row operator--(int) const;

protected:
    int rowPosition;
    const AbstractConstantTable* table;

    Row(const AbstractConstantTable* cnt, int pos);
};

class RowContainer {
friend class ConstantTable;
public:
    int size() const;
    Row begin() const;
    Row end() const;

protected:
    const AbstractConstantTable* table;

    RowContainer(const AbstractConstantTable* table);
};

}
}

#endif //DATASET_TABLE_ROW_H
