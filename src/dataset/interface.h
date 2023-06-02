#ifndef DATASET_INTERFACES_H
#define DATASET_INTERFACES_H

#include "types.h"

namespace Vizzu
{
namespace Dataset
{

/**
* @brief This abstract class defines the basic interface for a constant series of data points with a fixed number of elements
* and a fixed value type. The owner() and id() functions are used to identify the dataset to which the series belongs.
* The getParam() and addParam() functions allow for arbitrary metadata to be associated with the series.
* The size(), type(), typeAt(), valueAt(), begin(), and end() functions provide access to the data points themselves.
*/
class AbstractConstantSeries
{
public:
    /**
    @brief Returns the dataset that owns this constant series.
    @return A reference to the owning dataset.
    */
    virtual Dataset& owner() const = 0;

    /**
    * @brief Returns the ID of this constant series. The id is a 64 bit hash value based on the series name,
    * @return The ID of the constant series.
    */
    virtual DatasetId id() const = 0;
    
    /**
    * @brief Returns the name of this constant series.
    * @return The name of the constant series.
    */
    virtual const char* name() const = 0;
    
    /**
    * @brief Returns the value of the specified parameter.
    * @param name The name of the parameter.
    * @return The value of the specified parameter, or an empty string if the parameter is not set.
    */
    virtual const char* getParam(const char* name) const = 0;
    
    /**
    * @brief Sets the value of the specified parameter.
    * @param name The name of the parameter.
    * @param value The value of the parameter.
    */
    virtual void addParam(const char* name, const char* value) = 0;
    
    /**
    * @brief Returns the size of this constant series.
    * @return The size of the constant series.
    */
    virtual int size() const = 0;
    
    /**
    * @brief Returns the value type of this constant series.
    * @return The value type of the constant series.
    */
    virtual ValueType type() const = 0;
    
    /**
    * @brief Returns the value type of the element at the specified index.
    * @param index The index of the element.
    * @return The value type of the element at the specified index.
    */
    virtual ValueType typeAt(int index) const = 0;
    
    /**
    * @brief Returns the value of the element at the specified index.
    * @param index The index of the element.
    * @return The value of the element at the specified index.
    */
    virtual Value valueAt(int index) const = 0;
    
    /**
    * @brief Returns an iterator pointing to the first element of this constant series.
    * @return An iterator pointing to the first element of the constant series.
    */
    virtual ValueIterator begin() const = 0;
    
    /**
    * @brief Returns an iterator pointing to the end of this constant series.
    * @return An iterator pointing to the end of the constant series.
    */
    virtual ValueIterator end() const = 0;
    
    /**
    * @brief Virtual destructor.
    */
    virtual ~AbstractConstantSeries() {}
};

/**
* @brief The AbstractVolatileSeries interface defines a set of methods to modify the contents of a series
* that can be dynamically updated with new data. These methods include inserting, modifying, and erasing
* elements at specific positions in the series. The interface also provides a method to extend the
* size of the series.
*/
class AbstractVolatileSeries {
public:
    /**
    @brief Extends the size of the series.
    @param size The size to extend the series by.
    */
    virtual void extend(int size) = 0;

    /**
    * @brief Inserts a double value into the series.
    * @param value The value to insert.
    * @param position The index at which to insert the value.
    */
    virtual void insert(double value, int position = nullpos) = 0;

    /**
    * @brief Inserts a discrete value into the series.
    * @param value The value to insert.
    * @param position The index at which to insert the value.
    */
    virtual void insert(const char* value, int position = nullpos) = 0;

    /**
    * @brief Inserts a value of the specified type into the series.
    * @param vt The type of value to insert.
    * @param value The value to insert.
    * @param position The index at which to insert the value.
    */
    virtual void insert(ValueType vt, const Value& value, int position = nullpos) = 0;

    /**
    * @brief Inserts an array of double values into the series.
    * @param values The values to insert.
    * @param position The index at which to insert the values.
    */
    virtual void insert(std::span<double> values, int position = nullpos) = 0;

    /**
    * @brief Modifies a double value in the series.
    * @param value The new value.
    * @param position The index of the value to modify.
    */
    virtual void modify(double value, int position) = 0;

    /**
    * @brief Modifies a string value in the series.
    * @param value The new value.
    * @param position The index of the value to modify.
    */
    virtual void modify(const char* value, int position) = 0;

    /**
    * @brief Modifies a value of the specified type in the series.
    * @param vt The type of value to modify.
    * @param value The new value.
    * @param position The index of the value to modify.
    */
    virtual void modify(ValueType vt, const Value& value, int position) = 0;

    /**
    * @brief Removes a value from the series.
    * @param index The index of the value to remove.
    */
    virtual void erase(int index) = 0;

    /**
    * @brief Destructor for the AbstractVolatileSeries class.
    */
    virtual ~AbstractVolatileSeries() {}
};

/**
* @brief The AbstractConstantTable class represents a read-only table of constant data.
* This class provides an interface to access information about a table of constant data.
* It allows access to the table's rows and columns as well as specific cells and their values.
*/
class AbstractConstantTable {
public:
    /**
    * @brief Returns the dataset that owns this table.
    * @return A reference to the dataset that owns this table.
    */
    virtual Dataset& owner() const = 0;

    /**
    * @brief Returns the unique identifier of this table.
    * @return The unique identifier of this table.
    */
    virtual DatasetId id() const = 0;

    /**
    * @brief Returns the name of this table.
    * @return The name of this table.
    */
    virtual const char* name() const = 0;

    /**
    * @brief Returns the value of the specified parameter.
    * @param name The name of the parameter.
    * @return The value of the specified parameter.
    */
    virtual const char* getParam(const char* name) const = 0;

    /**
    * @brief Returns the number of rows in this table.
    * @return The number of rows in this table.
    */
    virtual int rowCount() const = 0;

    /**
    * @brief Returns a container that holds all the rows in this table.
    * @return A container that holds all the rows in this table.
    */
    virtual RowContainer rows() const = 0;

    /**
    * @brief Returns the number of columns in this table.
    * @return The number of columns in this table.
    */
    virtual int columnCount() const = 0;

    /**
    * @brief Returns a container that holds all the columns in this table.
    * @return A container that holds all the columns in this table.
    */
    virtual ColumnContainer cols() const = 0;

    /**
    * @brief Returns the row at the specified position.
    * @param pos The position of the row.
    * @return The row at the specified position.
    */
    virtual Row row(int pos) const = 0;

    /**
    * @brief Returns the column at the specified position.
    * @param pos The position of the column.
    * @return The column at the specified position.
    */
    virtual Column col(int pos) const = 0;

    /**
    * @brief Returns the cell at the specified column and row.
    * @param col The column of the cell.
    * @param row The row of the cell.
    * @return The cell at the specified column and row.
    */
    virtual Cell cell(int col, int row) const = 0;

    /**
    * @brief Returns the value of the cell at the specified column and row.
    * @param col The column of the cell.
    * @param row The row of the cell.
    * @return The value of the cell at the specified column and row.
    */
    virtual Value value(int col, int row) const = 0;

    /**
    * @brief Returns the data type of the cell at the specified column and row.
    * @param col The column of the cell.
    * @param row The row of the cell.
    * @return The data type of the cell at the specified column and row.
    */
    virtual ValueType valueType(int col, int row) const = 0;

    /**
     * @brief The rowId function in the ConstantTable class computes a unique identifier for a row in the table based
     * on the discrete values in each column. It iterates over all columns and multiplies the existing hash value by
     * the bitwise XOR of the column ID and the hash value of the value at the current row in that column.
     * If a column has a non-discrete value type, it is skipped in the computation.
     * @param row The row index.
     * @return The ID of the dataset that owns the given row.
     */
    virtual DatasetId rowId(int row) const = 0;

    /**
     * @brief Returns a unique identifier for the value at the specified column and row.
     * The identifier is calculated by multiplying the ID of the series by a hash of the
     * value's contents. For discrete values, the hash is calculated from the content
     * of the value. For continuous values, the hash is calculated from the raw bytes of the
     * value's memory representation.
     * @param col The column index.
     * @param row The row index.
     * @return The ID of the dataset that owns the value at the given column and row.
     */
    virtual DatasetId valueId(int col, int row) const = 0;

    /**
     * @brief Returns a constant series associated with the given name and type.
     * If the type parameter is set to TableSeriesType::unfiltered and the table has a sorter and filter applied,
     * the function retrieves the original unfiltered series.
     * If the type parameter is set to a value other than TableSeriesType::final and the table has either
     * a sorter or filter applied, the function retrieves the original unsorted or unfiltered.
     * @param name The name of the series to retrieve.
     * @param type The type of the series to retrieve.
     * @return A pointer to the constant series based on the given name and type.
     */
    virtual ConstantSeriesPtr getSeries(
            const char* name,
            TableSeriesType type = TableSeriesType::final) const = 0;

    /**
     * @brief Virtual destructor.
     */
    virtual ~AbstractConstantTable() {};
};

/**
 * @brief Interface for a volatile table, which allows modifying the contents of the table.
 */
class AbstractVolatileTable {
public:
    /*
     * @brief Inserts a new row at the specified position.
     * @param pos The position to insert the new row at.
     */
    virtual void insertRow(int pos) = 0;

    /**
     * @brief Removes the row at the specified position.
     * @param pos The position of the row to remove.
     */
    virtual void removeRow(int pos) = 0;

    /**
     * @brief Inserts a new column at the specified position with the specified series.
     * @param pos The position to insert the new column at.
     * @param ptr The constant series to insert as a new column.
     */
    virtual void insertColumn(int pos, const ConstantSeriesPtr& ptr) = 0;

    /**
     * @brief Removes the column at the specified position.
     * @param pos The position of the column to remove.
     * @param ptr The constant series associated with the column to remove.
     */
    virtual void removeColumn(int pos, const ConstantSeriesPtr& ptr) = 0;

    /**
     * @brief Adds a new parameter with the specified name and value.
     * @param name The name of the parameter to add.
     * @param value The value of the parameter to add.
     */
    virtual void addParam(const char* name, const char* value) = 0;

    /**
     * @brief Virtual destructor to allow proper cleanup of derived classes.
     */
    virtual ~AbstractVolatileTable() {};
};

/**
 * @brief Interface for a volatile table, which allows modifying the contents of the table.
 */
class AbstractTableGenerator {
public:
    /**
     * @brief TableOperations class is responsible for preparing and manipulating a table.
     */
    class TableOperations {
    public:
        /**
         * @brief Prepares for the insertion of a series.
         * @param seriesCount The number of series to be inserted.
         */
        virtual void prepare(int seriesCount) = 0;

        /**
         * @brief Inserts a series into the table.
         * @param ptr A constant series pointer.
         * @return The index of the inserted series.
         */
        virtual int insert(const ConstantSeriesPtr& ptr) = 0;

        /**
         * @brief Finalizes the table.
         * @param range The range of the table.
         */
        virtual void finalize(const RangePtr&) = 0;

        /**
         * @brief Virtual destructor to allow proper cleanup of derived classes.
         */
        virtual ~TableOperations() {};
    };

    using output_table_ptr = std::shared_ptr<TableOperations>;

public:
    /** @brief Generate the table according to the specifications of the generator.
     * This function generates the table by performing the necessary operations based on
     * the specific implementation of the generator. The TableOperations interface will
     * be called from this function. It is recommended to call the
     * setOutput function before calling generate in order to set up the output table.
     */
    virtual void generate() = 0;

    /**
     * @brief Sets the output table.
     *
     * @param output A shared pointer to the TableOperations object.
     */
    virtual void setOutput(const output_table_ptr& output) = 0;

    /**
     * @brief Virtual destructor to allow proper cleanup of derived classes.
     */
    virtual ~AbstractTableGenerator() {}
};

/**
 * @brief Interface for generating series.
 * This interface defines methods for setting up and generating series.
 */
class AbstractSeriesGenerator {
public:
    /**
     * @brief Set up the generator with the provided dataset.
     * @param ds The dataset to set up the generator with.
     */
    virtual void setup(const Dataset& ds) = 0;

    /**
     * @brief Get the value type of the record.
     * @param record The record to get the value type for.
     * @return The value type of the record.
     */
    virtual ValueType getType(int record) = 0;

    /**
     * @brief Get the value of the record.
     * @param record The record to get the value for.
     * @return The value of the record.
     */
    virtual Value getValue(int record) = 0;

    /**
     * @brief Virtual destructor.
     */
    virtual ~AbstractSeriesGenerator() {}
};

/**
 * @brief The AbstractSeriesAggregator class is an interface for aggregating a series of values.
 * It defines methods for setting up the aggregator, specifying the value type of the series,
 * aggregating each record in the series, and obtaining the final aggregated value.
 */
class AbstractSeriesAggregator {
public:
    /*
    * @brief Setup the aggregator with the given dataset.
    * This method is called before aggregating any records, and is used to initialize the aggregator
    * with any necessary information from the dataset.
    * @param ds The dataset to be aggregated.
    */
    virtual void setup(const Dataset& ds) = 0;

    /**
     * @brief Aggregate a record in the series.
     * This method is called once for each record in the series being aggregated.
     * @param index The index of the record to be aggregated.
     */
    virtual void aggregateRecord(int index) = 0;

    /**
     * @brief Get the final aggregated value.
     * This method returns the final aggregated value of the series.
     * @return The final aggregated value of the series.
     */
    virtual Value aggregatedValue() = 0;

    /**
     * @brief Get the value type of the aggregated value.
     * This method returns the value type of the aggregated value.
     * @return The type of the value.
    */
    virtual ValueType aggregatedType() = 0;

    /**
     * @brief Destroy the AbstractSeriesAggregator object.
     * This method is responsible for freeing any resources used by the aggregator.
     */
    virtual ~AbstractSeriesAggregator() {}
};

/**
 * @brief Interface for a filter that can be applied on a dataset or table.
 */
class AbstractFilter {
public:
    /**
     * @brief Sets up the filter based on the provided dataset.
     * @param ds The dataset to be filtered.
     */
    virtual void setup(const Dataset& ds) = 0;

    /**
     * @brief Sets up the filter based on the provided table.
     * @param table The table to be filtered.
     */
    virtual void setup(const ConstantTablePtr& table) = 0;

    /**
     * @brief Determines if a record should be included or excluded from the result.
     * @param recordIndex The index of the record to be filtered.
     * @return True if the record should be included in the result, false otherwise.
     */
    virtual bool filterRecord(int recordIndex) = 0;

    /**
     * @brief Virtual destructor to ensure proper cleanup of derived classes.
     */
    virtual ~AbstractFilter() {}
};

/**
 * @brief An abstract interface for sorting the rows of a constant table.
 * This interface defines the methods for sorting the rows of a constant table.
 * The sorting is done in-place, i.e. the original table is modified. The result of
 * the sorting can be obtained by creating an iterator through the sorted rows.
 */
class AbstractTableSorter {
public:
    /*
    * @brief An abstract interface for iterating over the sorted rows of a table.
    */
    class Iterator {
    public:
        /*
        * @brief Tests if there are more sorted rows to iterate over.
        * @return True if there are more rows, false otherwise.
        */
        virtual bool test() = 0;

        /**
         * @brief Advances the iterator to the next row.
         */
        virtual void next() = 0;

        /**
         * @brief Returns the index of the current row in the sorted table.
         * @return The index of the current row.
         */
        virtual int index() const = 0;

        /**
         * @brief Virtual destructor to ensure proper cleanup of derived classes.
         */
        virtual ~Iterator() {};
    };

    /**
     * @brief A shared pointer to an iterator object.
     */
    using iterator_ptr = std::shared_ptr<Iterator>;

    public:
    /**
    * @brief Sets up the sorter with the constant table to sort.
    * @param table The constant table to sort.
    */
    virtual void setup(const ConstantTablePtr& table) = 0;

    /**
     * @brief Sorts the row with the given index.
     * @param index The index of the row to sort.
     */
    virtual void sortRecord(int index) = 0;

    /**
     * @brief Returns an iterator over the sorted rows.
     * @return A shared pointer to an iterator object.
     */
    virtual iterator_ptr result() = 0;

    /**
     * @brief Virtual destructor to ensure proper cleanup of derived classes.
     */
    virtual ~AbstractTableSorter() {}
};

} // namespace Dataset
} // namespace Vizzu
#endif // DATASET_INTERFACES_H
