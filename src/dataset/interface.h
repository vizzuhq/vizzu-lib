#ifndef DATAFRAME_INTERFACES_H
#define DATAFRAME_INTERFACES_H

#include "types.h"

namespace Vizzu
{
namespace Dataset
{

class AbstractConstantSeries
{
public:
	virtual Dataset& owner() const = 0;
	virtual DatasetId id() const = 0;
	virtual const char* name() const = 0;
	virtual int size() const = 0;
	virtual ValueType type() const = 0;
	virtual ValueType typeAt(int index) const = 0;
	virtual Value valueAt(int index) const = 0;
	virtual ValueIterator begin() const = 0;
	virtual ValueIterator end() const = 0;
	
	virtual ~AbstractConstantSeries() {}
};

class AbstractVolatileSeries {
public:
    virtual void extend(int size) = 0;
    virtual void insert(double value, int position = nullpos) = 0;
    virtual void insert(const char* value, int position = nullpos) = 0;
    virtual void insert(ValueType vt, const Value& value, int position = nullpos) = 0;
    virtual void modify(double value, int position) = 0;
    virtual void modify(const char* value, int position) = 0;
    virtual void modify(ValueType vt, const Value& value, int position) = 0;
    virtual void erase(int index) = 0;

	virtual ~AbstractVolatileSeries() {}
};

class AbstractConstantTable {
public:
	virtual Dataset& owner() const = 0;
	virtual DatasetId id() const = 0;
    virtual const char* name() const = 0;
    virtual int rowCount() const = 0;
    virtual RowContainer rows() const = 0;
    virtual int columnCount() const = 0;
    virtual ColumnContainer cols() const = 0;
    virtual Row row(int pos) const = 0;
    virtual Column col(int pos) const = 0;
    virtual Cell cell(int col, int row) const = 0;
    virtual Value value(int col, int row) const = 0;
    virtual ValueType valueType(int col, int row) const = 0;
    virtual DatasetId valueId(int col, int row) const = 0;

	virtual ~AbstractConstantTable() {};
};

class AbstractVolatileTable {
public:
    virtual void insertRow(int pos) = 0;
    virtual void removeRow(int pos) = 0;
    virtual void insertColumn(int pos, const ConstantSeriesPtr& ptr) = 0;
    virtual void removeColumn(int pos, const ConstantSeriesPtr& ptr) = 0;

	virtual ~AbstractVolatileTable() {};
};

class AbstractTableGenerator {
public:
    class TableOperations {
    public:
        virtual void prepare(int seriesCount) = 0;
        virtual int insert(const ConstantSeriesPtr& ptr) = 0;
        virtual void finalize() = 0;

        virtual ~TableOperations() {};
    };

    using output_table_ptr = std::shared_ptr<TableOperations>;

public:
    virtual void generate() = 0;
    virtual void setOutput(const output_table_ptr& output) = 0;
    virtual ~AbstractTableGenerator() {}
};

class AbstractSeriesGenerator {
public:
    virtual void setup(const Dataset& ds) =  0;
    virtual ValueType type() = 0;
    virtual Value generateRecord(int record) = 0;
    virtual ~AbstractSeriesGenerator() {}
};

class AbstractSeriesAggregator {
public:
    virtual void setup(const Dataset& ds) = 0;
    virtual ValueType type() = 0;
    virtual void aggregateRecord(int index) = 0;
    virtual Value aggregatedValue() = 0;
    virtual ~AbstractSeriesAggregator() {}
};

class AbstractFilter {
public:
    virtual void setup(const Dataset& ds) = 0;
    virtual bool filterRecord(int recordIndex) = 0;
	virtual ~AbstractFilter() {}
};

class AbstractSorter {
public:
    class Iterator {
    public:
        virtual bool test() = 0;
        virtual void next() = 0;
        virtual int index() const = 0;
        virtual ~Iterator() {};
    };

    using iterator_ptr = std::shared_ptr<Iterator>;

public:
    virtual void setup(const SeriesContainer& sc) = 0;
    virtual void sortRecord(int index) = 0;
    virtual iterator_ptr result() = 0;
	virtual ~AbstractSorter() {}
};

} // namespace Dataset
} // namespace Vizzu
#endif // DATAFRAME_TYPES_H
