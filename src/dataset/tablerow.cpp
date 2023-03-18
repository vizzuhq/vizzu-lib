#include "table.h"
#include "tablerow.h"
#include "tablecol.h"

namespace Vizzu
{
namespace Dataset
{

int Row::position() const {
    return rowPosition;
}

CellIterator Row::begin() const {
    return CellIterator{table, true, 0, rowPosition};
}

CellIterator Row::end() const {
    return CellIterator{table, true, table->columnCount(), rowPosition};
}

Row::Row(const AbstractConstantTable* cnt, int pos)
    : rowPosition(pos), table(cnt)
{
}

RowIterator RowIterator::operator++(int) const {
    return RowIterator{table, position + 1};
}

RowIterator& RowIterator::operator++() {
    if (position < table->rows().size())
        position++;
    return *this;
}

RowIterator RowIterator::operator--(int) const {
    return RowIterator{table, position - 1};
}

RowIterator& RowIterator::operator--() {
    position--;
    if (position < 0)
        position = table->rows().size();
    return *this;
}

bool RowIterator::operator==(const RowIterator& arg) const {
    return position == arg.position && table == arg.table;
}

bool RowIterator::operator!=(const RowIterator& arg) const {
    return position != arg.position || table != arg.table;
}

Row RowIterator::operator*() const {
    return Row{table, position};
}

RowIterator::RowIterator(const AbstractConstantTable* table, int pos)
    : position(pos), table(table)
{
    if (pos < 0 || pos > table->rows().size())
        position = table->rows().size();
}

int RowContainer::size() const {
    return table->rowCount();
}

RowIterator RowContainer::begin() const {
    return RowIterator{table, 0};
}

RowIterator RowContainer::end() const {
    return RowIterator{table, size()};
}

RowContainer::RowContainer(const AbstractConstantTable* table)
    : table(table)
{
}

}
}
