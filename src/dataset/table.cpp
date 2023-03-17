#include "table.h"
#include "tablerow.h"
#include "tablecol.h"
#include "tablecell.h"

namespace Vizzu
{
namespace Dataset
{

ConstantTable::ConstantTable(Dataset& dataset, DatasetId id, const char* name)
    : dataset(dataset), tableId(id), tableName(name)
{
}

Dataset& ConstantTable::owner() const {
    return dataset;
}

const char* ConstantTable::name() const {
    return tableName.c_str();
}

DatasetId ConstantTable::id() const {
    return tableId;
}

RowContainer ConstantTable::rows() const {
    return RowContainer{this};
}

ColumnContainer ConstantTable::cols() const {
    return ColumnContainer{this};
}

Row ConstantTable::row(int) const {
    return Row{this, 0};
}

Column ConstantTable::col(int) const {
    return Column{this, 0};
}

Cell ConstantTable::cell(int row, int col) const {
    return Cell{this, row, col};
}

void Table::insertRow(int) {
}

void Table::removeRow(int) {
}

void Table::insertColumn(int, const ConstantSeriesPtr&) {
}

void Table::removeColumn(int, const ConstantSeriesPtr&) {
}

void GeneratedTable::setSorter(const SorterPtr& sptr) {
    sorter = sptr;
}

void GeneratedTable::setFilter(const FilterPtr& fptr) {
    filter = fptr;
}

void GeneratedTable::refresh() {
    auto ptr = shared_from_this();
    auto output = std::dynamic_pointer_cast<AbstractTableGenerator::Operations>(ptr);
    generator->setOutput(output);
    generator->generate();
}

void GeneratedTable::prepare(int seriesCount) {
    series.reserve(seriesCount);
}

int GeneratedTable::insert(const ConstantSeriesPtr& sptr) {
    series.push_back(sptr);
    return series.size() - 1;
}

}
}
