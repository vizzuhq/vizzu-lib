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

void GeneratedTable::setSorter(const SorterPtr& sptr) {
    sorter = sptr;
    applySorter();
}

void GeneratedTable::setFilter(const FilterPtr& fptr) {
    filter = fptr;
    applyFilter();
}

void GeneratedTable::refresh() {
    auto ptr = shared_from_this();
    auto output = std::dynamic_pointer_cast<AbstractTableGenerator::TableOperations>(ptr);
    generator->setOutput(output);
    generator->generate();
}

void GeneratedTable::prepare(int seriesCount) {
    series.reserve(seriesCount);
}

int GeneratedTable::insert(const ConstantSeriesPtr& sptr) {
    auto newSeries = sptr;
    if (filter) {
        auto linkedSeries = std::make_shared<LinkedSeries>(newSeries);
        if (!filteredIndeces)
            filteredIndeces = linkedSeries->createSelector(newSeries->size());
        else
            linkedSeries->setSelector(filteredIndeces);
        newSeries = linkedSeries;
    }
    if (sorter) {
        auto sortedSeries = std::make_shared<LinkedSeries>(newSeries);
        if (!sortedIndeces)
            sortedIndeces = sortedSeries->createSelector(newSeries->size());
        else
            sortedSeries->setSelector(sortedIndeces);
        newSeries = sortedSeries;
    }
    series.push_back(newSeries);
    seriesByName.insert(std::make_pair(newSeries->name(), SeriesItem{newSeries, RangePtr{}}));
    return series.size() - 1;
}

void GeneratedTable::finalize() {
    applyFilter();
    applySorter();
}

void GeneratedTable::applyFilter() {
    if (filter && filteredIndeces) {
        filteredIndeces->clear();
        filter->setup(dataset);
        auto lptr = std::dynamic_pointer_cast<LinkedSeries>(series[0]);
        if (sorter)
            lptr = std::dynamic_pointer_cast<LinkedSeries>(lptr->originalSeries());
        auto recordCount = lptr->originalSize(); 
        filteredIndeces->reserve(recordCount);
        for(int ai = 0; ai < recordCount; ai++) {
            if (filter->filterRecord(ai))
                lptr->select(ai);
        }
        filteredIndeces->shrink_to_fit();
    }
}

void GeneratedTable::applySorter() {
    if (sorter && sortedIndeces) {
        sortedIndeces->clear();
        sorter->setup(seriesByName);
        auto lptr = std::dynamic_pointer_cast<LinkedSeries>(series[0]);
        auto recordCount = lptr->size();
        sortedIndeces->reserve(recordCount);
        for(int ai = 0; ai < recordCount; ai++)
            sorter->sortRecord(ai);
        auto iter = sorter->result();
        while(iter->test()) {
            lptr->select(iter->index());
            iter->next();
        }
        sortedIndeces->shrink_to_fit();
    }
}

}
}
