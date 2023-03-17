#include "tablecell.h"
#include "tablecol.h"
#include "tablerow.h"

namespace Vizzu
{
namespace Dataset
{

Row Cell::row() const {
    return Row{table, rowPosition};
}

Column Cell::col() const {
    return Column{table, colPosition};
}

void Cell::nextCol() {
}

void Cell::prevCol() {
}

void Cell::nextRow() {
}

void Cell::prevRow() {
}

const Value& Cell::operator*() {
    static Value empty;
    return empty;
}

Cell::Cell(const AbstractConstantTable* table, int row, int col) :
    colPosition(row), rowPosition(col), table(table)
{
}

}
}