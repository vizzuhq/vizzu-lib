#include "generatedtable.h"
#include "tablerow.h"
#include "tablecol.h"
#include "tablecell.h"

namespace Vizzu
{
namespace Dataset
{

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
            seriesByName.insert(std::make_pair(csPtr->name(), csPtr));
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
            seriesByName.insert(std::make_pair(csPtr->name(), csPtr));
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
