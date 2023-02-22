#ifndef DATASET_TABLE_ROW_H
#define DATASET_TABLE_ROW_H

#include "types.h"
#include "tablecell.h"

namespace Vizzu
{
namespace DataSet
{

class Row {
friend class Cell;
friend class Table;
friend class RowContainer;
public:
    int position() const;
    Cell begin() const;
    Cell end() const;
    Row operator++(int) const;
    Row operator--(int) const;

protected:
    int rowPosition;
    ConstTablePtr table;

    Row(const ConstTablePtr& cnt, int pos);
};

class RowContainer {
friend class Table;
public:
    int size() const;
    Row begin() const;
    Row end() const;

protected:
    ConstTablePtr table;

    RowContainer(const ConstTablePtr& table);
};

}
}

#endif //DATASET_TABLE_ROW_H
