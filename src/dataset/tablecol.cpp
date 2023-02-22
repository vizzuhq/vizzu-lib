#include "tablecol.h"
#include "table.h"
#include "tablerow.h"

namespace Vizzu
{
namespace DataSet
{

int Column::position() const {
    return colPosition;
}

Cell Column::begin() const {
    return table->cell(colPosition, 0);
}

Cell Column::end() const {
    return table->cell(colPosition, table->rows().size());
}

Column Column::operator++(int) const {
    return Column{table, colPosition + 1};
}

Column Column::operator--(int) const {
    return Column{table, colPosition - 1};
}

Column::Column(const ConstTablePtr& table, int pos)
    : colPosition(pos), table(table)
{
    if (pos < 0 || pos > table->cols().size())
        pos = table->cols().size();
}

int ColumnContainer::size() const {
    return table->series.size();
}

Column ColumnContainer::begin() const {
    return Column{table, 0};
}

Column ColumnContainer::end() const {
    return Column{table, size()};
}

ColumnContainer::ColumnContainer(const ConstTablePtr& table)
    : table(table)
{
}

}
}
