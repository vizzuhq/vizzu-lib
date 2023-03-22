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
    resetSorterIndeces();
    sorter = sptr;
    prepareSorterIndices();
    applySorter();
}

void GeneratedTable::setFilter(const FilterPtr& fptr) {
    resetSorterIndeces();
    resetFilterIndeces();
    filter = fptr;
    prepareFilterIndices();
    prepareSorterIndices();
    applyFilter();
    applySorter();
}

void GeneratedTable::refresh() {
    auto ptr = shared_from_this();
    auto output = std::dynamic_pointer_cast<AbstractTableGenerator::TableOperations>(ptr);
    generator->setOutput(output);
    generator->generate();
}

void GeneratedTable::prepare(int seriesCount) {
    resetSorterIndeces();
    resetFilterIndeces();
    series.reserve(seriesCount);
}

int GeneratedTable::insert(const ConstantSeriesPtr& sptr) {
    series.push_back(sptr);
    return series.size() - 1;
}

void GeneratedTable::finalize() {
    prepareFilterIndices();
    prepareSorterIndices();
    applyFilter();
    applySorter();
}

void GeneratedTable::resetSorterIndeces() {
    if (sortedIndeces) {
        sortedIndeces->clear();
        for(auto& ptr1 : series) {
            auto ptr2 = std::dynamic_pointer_cast<LinkedSeries>(ptr1);
            ptr1 = ptr2->originalSeries();
        }
    }
}

void GeneratedTable::prepareSorterIndices() {
    if (!sortedIndeces && sorter) {
        seriesByName.clear();
        for(auto& csPtr : series) {
            auto sortedSeries = std::make_shared<LinkedSeries>(csPtr);
            if (!sortedIndeces)
                sortedIndeces = sortedSeries->createSelector(csPtr->size());
            else
                sortedSeries->setSelector(sortedIndeces);
            csPtr = sortedSeries;
            seriesByName.insert(std::make_pair(csPtr->name(), SeriesItem{csPtr, RangePtr{}}));
        }
    }
}

void GeneratedTable::applySorter() {
    if (sortedIndeces) {
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

void GeneratedTable::resetFilterIndeces() {
    if (filteredIndeces) {
        filteredIndeces->clear();
        for(auto& ptr1 : series) {
            auto ptr2 = std::dynamic_pointer_cast<LinkedSeries>(ptr1);
            ptr1 = ptr2->originalSeries();
        }
    }
}

void GeneratedTable::prepareFilterIndices() {
    if (!filteredIndeces && filter) {
        seriesByName.clear();
        for(auto& csPtr : series) {
            auto linkedSeries = std::make_shared<LinkedSeries>(csPtr);
            if (!filteredIndeces)
                filteredIndeces = linkedSeries->createSelector(csPtr->size());
            else
                linkedSeries->setSelector(filteredIndeces);
            csPtr = linkedSeries;        
            seriesByName.insert(std::make_pair(csPtr->name(), SeriesItem{csPtr, RangePtr{}}));
        }
    }
}

void GeneratedTable::applyFilter() {
    if (filteredIndeces) {
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

}
}
