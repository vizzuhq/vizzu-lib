#include "table.h"
#include "tablerow.h"
#include "tablecol.h"

namespace Vizzu
{
namespace DataSet
{

int Row::position() const {
    return rowPosition;
}

Cell Row::begin() const {
    return table->cell(0, rowPosition);
}

Cell Row::end() const {
    return table->cell(table->cols().size(), rowPosition);
}

Row Row::operator++(int) const {
    return Row{table, rowPosition + 1};
}

Row Row::operator--(int) const {
    return Row{table, rowPosition - 1};
}

Row::Row(const ConstTablePtr& table, int pos)
    : rowPosition(pos), table(table)
{
    if (pos < 0 || pos > table->rows().size())
        pos = table->rows().size();
}

int RowContainer::size() const {
    return table->index->size();
}

Row RowContainer::begin() const {
    return Row{table, 0};
}

Row RowContainer::end() const {
    return Row{table, size()};
}

RowContainer::RowContainer(const ConstTablePtr& table)
    : table(table)
{
}

}
}
