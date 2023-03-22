#ifndef DATASET_GENERATEDTABLE_H
#define DATASET_GENERATEDTABLE_H

#include "table.h"

namespace Vizzu {
namespace Dataset {

class GeneratedTable :
    public ConstantTable,
    public AbstractTableGenerator::TableOperations,
    public std::enable_shared_from_this<GeneratedTable>
{
friend class RowContainer;
friend class ColumnContainer;
public:
    template<typename T>
    GeneratedTable(Dataset& ds, const char* name, const T& gptr)
        : ConstantTable(ds, name)
    {
        generator = std::dynamic_pointer_cast<AbstractTableGenerator>(gptr);
    }

    void setSorter(const SorterPtr& ptr);
    void setFilter(const FilterPtr& ptr);
    void refresh();

protected:
    TableGeneratorPtr generator;
    LinkedSeries::indices_ptr filteredIndeces;
    LinkedSeries::indices_ptr sortedIndeces;

    void prepare(int seriesCount) override;
    int insert(const ConstantSeriesPtr& ptr) override;
    void finalize() override;
    void resetSorterIndeces();
    void prepareSorterIndices();
    void applySorter();
    void resetFilterIndeces();
    void prepareFilterIndices();
    void applyFilter();
};

}
}

#endif //DATASET_GENERATEDTABLE_H
