#include "table.h"
#include "tablerow.h"
#include "tablecol.h"
#include "tablecell.h"

namespace Vizzu
{
namespace DataSet
{

Table::Table(DataSet&) {
}

void Table::setSorter(const SorterPtr&) {
}

void Table::setFilter(const FilterPtr&) {
}

void Table::insertRow(int) {
}

void Table::removeRow(int) {
}

void Table::insertColumn(int, SeriesPtr) {
}

void Table::removeColumn(int, SeriesPtr) {
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

TableContainer::TableContainer() {
}

int TableContainer::size() {
    return 0;
}

TableIterator TableContainer::begin() {
    return TableIterator{};
}

TableIterator TableContainer::end() {
    return TableIterator{};
}

}
}
