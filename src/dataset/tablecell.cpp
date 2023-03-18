#include "table.h"

namespace Vizzu
{
namespace Dataset
{

int Cell::rowPos() const {
    return row;
}

int Cell::colPos() const {
    return column;
}

Value Cell::value() const {
    return table->value(column, row);
}

ValueType Cell::type() const {
    return table->valueType(column, row);
}

DatasetId Cell::id() const {
    return table->valueId(column, row);
}

Cell::Cell(const AbstractConstantTable* table, int col, int row)
    : row(row), column(col), table(table)
{
}

CellIterator CellIterator::operator++(int) const {
    return direction ?  
        CellIterator{table, direction, column + 1, row} :
        CellIterator{table, direction, column, row + 1};
}

CellIterator& CellIterator::operator++() {
    if (direction)
        column = column < table->columnCount() ? column + 1 : column;
    else
        row = row < table->rowCount() ? row + 1 : row;
    return *this;
}

CellIterator CellIterator::operator--(int) const {
    return direction ?
        CellIterator{table, direction, column - 1, row} :
        CellIterator{table, direction, column, row - 1};
}

CellIterator& CellIterator::operator--() {
   if (direction)
        column = column >= 0 ? column - 1 : table->columnCount();
    else
        row = row >= 0 ? row - 1 : table->rowCount();
    return *this;
}

bool CellIterator::operator==(const CellIterator& arg) const {
    return row == arg.row && column == arg.column;
}

bool CellIterator::operator!=(const CellIterator& arg) const {
    return row != arg.row || column != arg.column;
}

Cell CellIterator::operator*() const {
    return table->cell(column, row);
}

CellIterator::CellIterator(const AbstractConstantTable* table, bool dir, int col, int row)
    : row(row), column(col), direction(dir), table(table)
{
    if (col < 0 || col > table->columnCount())
        col = table->columnCount();
    if (row < 0 || row > table->rowCount())
        row = table->rowCount();
}

}
}