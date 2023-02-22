#ifndef DATASET_TABLE_COLUMN_H
#define DATASET_TABLE_COLUMN_H

#include "types.h"
#include "tablecell.h"

namespace Vizzu
{
namespace DataSet
{

class Column {
friend class Cell;
friend class Table;
friend class ColumnContainer;
public:
    int position() const;
    Cell begin() const;
    Cell end() const;
    Column operator++(int) const;
    Column operator--(int) const;

protected:
    int colPosition;
    ConstTablePtr table;

    Column(const ConstTablePtr& table, int pos);
};

class ColumnContainer {
friend class Table;
public:
    int size() const;
    Column begin() const;
    Column end() const;

protected:
    ConstTablePtr table;

    ColumnContainer(const ConstTablePtr& table);
};

}
}

#endif //DATASET_TABLE_COLUMN_H
