#include "table.h"
#include "tablerow.h"
#include "tablecol.h"
#include "tablecell.h"

namespace Vizzu
{
namespace Dataset
{

Table::Table(Dataset& dataset, const char* name)
    : dataset(dataset), tableId(nullid), tableName(name)
{
}

void Table::setSorter(const TableSorterPtr&) {
}

void Table::setFilter(const TableFilterPtr&) {
}

void Table::setGenerator(const TableGeneratorPtr&) {
}

void Table::insertRow(int) {
}

void Table::removeRow(int) {
}

void Table::insertColumn(int, const ConstantSeriesPtr&) {
}

void Table::removeColumn(int, const ConstantSeriesPtr&) {
}

RowContainer Table::rows() const {
    return RowContainer{shared_from_this()};
}

ColumnContainer Table::cols() const {
    return ColumnContainer{shared_from_this()};
}

Row Table::row(int) const {
    return Row{shared_from_this(), 0};
}

Column Table::col(int) const {
    return Column{shared_from_this(), 0};
}

Cell Table::cell(int row, int col) const {
    return Cell{shared_from_this(), row, col};
}

}
}
