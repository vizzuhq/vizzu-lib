#ifndef DATASET_TABLE_H
#define DATASET_TABLE_H

#include "../types.h"
#include "../interface.h"
#include "../series/linkedseries.h"
#include "tablerow.h"
#include "tablecol.h"
#include "tablecell.h"

namespace Vizzu
{
namespace Dataset
{

class ConstantTable :
    public AbstractConstantTable
{
friend class Cell;
friend class RowContainer;
friend class ColumnContainer;
public:
    ConstantTable(Dataset& dataset, const char* name);

	Dataset& owner() const override;
    const char* name() const override;
	DatasetId id() const override;
    int rowCount() const override;
    RowContainer rows() const override;
    int columnCount() const override;
    ColumnContainer cols() const override;
    Row row(int pos) const override;
    Column col(int pos) const override;
    Cell cell(int col, int row) const override;
    Value value(int col, int row) const override;
    ValueType valueType(int col, int row) const override;
    DatasetId valueId(int col, int row) const override;

protected:
    using series_vector = std::vector<ConstantSeriesPtr>;

    Dataset& dataset;
    DatasetId tableId;
    std::string tableName;
    SorterPtr sorter;
    FilterPtr filter;
    series_vector series;
    SeriesContainer seriesByName;
};

class Table :
    public ConstantTable,
    public AbstractVolatileTable,
    public std::enable_shared_from_this<Table>
{
friend class RowContainer;
friend class ColumnContainer;
public:
    Table(Dataset& dataset, const char* name);

    void insertRow(int pos) override;
    void removeRow(int pos) override;
    void insertColumn(int pos, const ConstantSeriesPtr& ptr) override;
    void removeColumn(int pos, const ConstantSeriesPtr& ptr) override;
};

}
}

#endif //DATASET_TABLE_H
