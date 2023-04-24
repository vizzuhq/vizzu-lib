#include "table.h"
#include "tablecol.h"
#include "tablerow.h"

namespace Vizzu
{
namespace Dataset
{

int Column::position() const {
    return colPosition;
}

CellIterator Column::begin() const {
    return CellIterator{table, false, colPosition, 0};
}

CellIterator Column::end() const {
    return CellIterator{table, false, colPosition, table->rowCount()};
}

Column::Column(const AbstractConstantTable* table, int pos)
    : colPosition(pos), table(table)
{
}

ColumnIterator ColumnIterator::operator++(int) const {
    return ColumnIterator{table, position + 1};
}

ColumnIterator& ColumnIterator::operator++() {
    if (position < table->cols().size())
        position++;
    return *this;
}

ColumnIterator ColumnIterator::operator--(int) const {
    return ColumnIterator{table, position - 1};
}

ColumnIterator& ColumnIterator::operator--() {
    position--;
    if (position < 0)
        position = table->cols().size();
    return *this;
}

bool ColumnIterator::operator==(const ColumnIterator& arg) const {
    return position == arg.position && table == arg.table;
}

bool ColumnIterator::operator!=(const ColumnIterator& arg) const {
    return position != arg.position || table != arg.table;
}

Column ColumnIterator::operator*() const {
    return Column{table, position};
}

ColumnIterator::ColumnIterator(const AbstractConstantTable* table, int pos)
    : position(pos), table(table)
{
    if (pos < 0 || pos > table->cols().size())
        position = table->cols().size();
}

int ColumnContainer::size() const {
    return table->columnCount();
}

ColumnIterator ColumnContainer::begin() const {
    return ColumnIterator{table, 0};
}

ColumnIterator ColumnContainer::end() const {
    return ColumnIterator{table, size()};
}

ColumnContainer::ColumnContainer(const AbstractConstantTable* table)
    : table(table)
{
}

}
}
