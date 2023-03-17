#ifndef DATASET_TABLE_H
#define DATASET_TABLE_H

#include "types.h"
#include "interfaces.h"

namespace Vizzu
{
namespace Dataset
{

class ConstantTable :
    public AbstractConstantTable
{
friend class RowContainer;
friend class ColumnContainer;
public:
    ConstantTable(Dataset& dataset, DatasetId id, const char* name);

	Dataset& owner() const override;
    const char* name() const override;
	DatasetId id() const override;
    RowContainer rows() const override;
    ColumnContainer cols() const override;
    Row row(int pos) const override;
    Column col(int pos) const override;
    Cell cell(int col, int row) const override;

protected:
    using series_vector = std::vector<ConstantSeriesPtr>;

    Dataset& dataset;
    DatasetId tableId;
    std::string tableName;
    SorterPtr sorter;
    FilterPtr filter;
    series_vector series;
};

class Table :
    public ConstantTable,
    public AbstractVolatileTable,
    public std::enable_shared_from_this<Table>
{
friend class RowContainer;
friend class ColumnContainer;
public:
    Table(Dataset& dataset, DatasetId id, const char* name);

    void insertRow(int pos) override;
    void removeRow(int pos) override;
    void insertColumn(int pos, const ConstantSeriesPtr& ptr) override;
    void removeColumn(int pos, const ConstantSeriesPtr& ptr) override;
};

class GeneratedTable :
    public ConstantTable,
    public AbstractTableGenerator::Operations,
    public std::enable_shared_from_this<GeneratedTable>
{
friend class RowContainer;
friend class ColumnContainer;
public:
    template<typename T>
    GeneratedTable(Dataset& ds, DatasetId id, const char* name, const T& gptr)
        : ConstantTable(ds, id, name)
    {
        generator = std::dynamic_pointer_cast<AbstractTableGenerator>(gptr);
    }

    void setSorter(const SorterPtr& ptr);
    void setFilter(const FilterPtr& ptr);
    void refresh();

protected:
    TableGeneratorPtr generator;

    void prepare(int seriesCount);
    int insert(const ConstantSeriesPtr& ptr);
};

}
}

#endif //DATASET_TABLE_H
