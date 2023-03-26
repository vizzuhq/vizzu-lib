#include "table.h"
#include "tablerow.h"
#include "tablecol.h"
#include "tablecell.h"

namespace Vizzu
{
namespace Dataset
{

ConstantTable::ConstantTable(Dataset& dataset, const char* name)
    : dataset(dataset), tableId(nullid), tableName(name)
{
    tableId = DiscreteValue::hash(name);
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

const char* ConstantTable::getParam(const char* name) const {
    const auto& iter = parameters.find(name);
    if (iter == parameters.end())
        return nullptr;
    return iter->second.c_str();
}

ConstantSeriesPtr ConstantTable::getSeries(const char* name, TableSeriesType type) const {
    auto iter = seriesByName.find(name);
    if (iter == seriesByName.end())
        return ConstantSeriesPtr{};
    if (type == TableSeriesType::unfiltered && sorter && filter) {
        auto ptr1 = std::dynamic_pointer_cast<LinkedSeries>(iter->second);
        auto ptr2 = std::dynamic_pointer_cast<LinkedSeries>(ptr1->originalSeries());
        return ptr2->originalSeries();
    }
    if (type != TableSeriesType::final && (sorter || filter)) {
        auto ptr = std::dynamic_pointer_cast<LinkedSeries>(iter->second);
        return ptr->originalSeries();
    }
    return iter->second;
}

int ConstantTable::rowCount() const {
    if (series.size())
        return series[0]->size();
    else
        return 0;
}

RowContainer ConstantTable::rows() const {
    return RowContainer{this};
}

int ConstantTable::columnCount() const {
    return series.size();
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

Value ConstantTable::value(int col, int row) const {
    if ((int)series.size() > col && series[col]->size() > row)
        return series[col]->valueAt(row);
    return Value{};
}

ValueType ConstantTable::valueType(int col, int row) const {
    if ((int)series.size() > col && series[col]->size() > row)
        return series[col]->typeAt(row);
    return ValueType{};
}

DatasetId ConstantTable::rowId(int row) const {
    DatasetId hash = 0;
    for(auto& column : series) {
        if (column->type() != ValueType::discrete)
            continue;
        hash *= column->id() ^ column->valueAt(row).getd().hash();
    }
    return hash;
}

DatasetId ConstantTable::valueId(int col, int row) const {
    if ((int)series.size() > col && series[col]->size() > row) {
        if (series[col]->typeAt(row) == ValueType::discrete)
            return series[col]->id() * series[col]->valueAt(row).getd().hash();
        else
            return series[col]->id() * *(uint64_t*)&(series[col]->valueAt(row).getc());
    }
    return DatasetId{};
}

void Table::insertRow(int) {
}

void Table::removeRow(int) {
}

void Table::insertColumn(int, const ConstantSeriesPtr&) {
}

void Table::removeColumn(int, const ConstantSeriesPtr&) {
}

void Table::addParam(const char* name, const char* param) {
    parameters.insert(std::make_pair(name, param));
}

}
}
