#ifndef DATAFRAME_TYPES_H
#define DATAFRAME_TYPES_H

#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <functional>
#include <memory>

namespace Vizzu
{
namespace DataSet
{

typedef double ContinousValue;
typedef uint64_t ValueId;
typedef uint32_t SeriesId;
typedef uint32_t TableId;
typedef uint32_t DiscreteId;
typedef uint64_t RecordId;

class DiscreteValue;
class DiscreteValueContainer;
class Value;
class ValueIterator;

class AbstractSeries;
class Series;
class RawSeries;
class SeriesIterator;
class AbstractSeriesGenerator;
class OrdNumSeries;
class SeriesContainer;

class SeriesIndex;
class AbstractFilter;
class AbstractSorter;
class SingleColumnSorter;
class MultiColumnSorter;

class Cell;
class CellIterator;
class Row;
class RowIterator;
class RowContainer;
class Column;
class ColIterator;
class ColContainer;
class Table;

class AbstractTableBuilder;
class AbstractSeriesAggregator;
class Identical;
class Aggregator;
class Normalizer;
class Pivoter;

class TableIterator;
class TableContainer;
class DataSet;

struct DiscreteValueComparer {
    bool operator()(const DiscreteValue& ldv, const DiscreteValue& rdv) const;
};

enum class ValueType {
    null,
    discrete,
    continous
};

typedef std::function<std::string(const char*)> DiscreteValueLookupFn;
typedef std::function<bool(const char*, const char*)> DiscreteValueEqTestFn;

typedef std::shared_ptr<AbstractSeries> SeriesPtr;
typedef std::shared_ptr<SeriesIndex> SeriesIndexPtr;
typedef std::shared_ptr<Table> TablePtr;
typedef std::shared_ptr<AbstractTableBuilder> TableBuilderPtr;
typedef std::vector<Value> ValueVector;
typedef std::vector<AbstractSorter> SorterPtr;
typedef std::vector<AbstractFilter> FilterPtr;

typedef std::unordered_map<
    DiscreteId,
    DiscreteValue,
    std::hash<DiscreteId>,
    DiscreteValueComparer
> DiscreteValuesMap;

typedef std::unordered_map<
    SeriesId,
    SeriesPtr,
    std::hash<SeriesId>
> SeriesMap;

typedef std::unordered_map<
    TableId,
    TablePtr,
    std::hash<TableId>
> TableMap;

typedef std::unordered_map<
    std::string,
    std::string
> SeriesInfo;

typedef std::vector<
    SeriesPtr
> SeriesVector;

typedef std::vector<
    int
> IndexVector;

}
}

#endif //DATAFRAME_TYPES_H
