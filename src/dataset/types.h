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

const uint32_t nullSeriesId = 0;
const int nullpos = -1;

typedef double ContinousValue;
typedef uint64_t ValueId;
typedef uint32_t SeriesId;
typedef uint32_t TableId;
typedef uint64_t RecordId;
typedef uint32_t DiscreteHash;

class Value;
class ValueIterator;
class DiscreteValue;
class DiscreteValueContainer;

class AbstractSeries;
class Series;
class MutableSeries;
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

struct DiscreteValueHasher {
    size_t operator()(const char*) const;
    size_t operator()(uint32_t) const;
};

typedef std::shared_ptr<AbstractSeries> SeriesPtr;
typedef std::shared_ptr<MutableSeries> MutableSeriesPtr;
typedef std::shared_ptr<SeriesIndex> SeriesIndexPtr;
typedef std::shared_ptr<Table> TablePtr;
typedef std::shared_ptr<AbstractTableBuilder> TableBuilderPtr;
typedef std::vector<Value> ValueVector;
typedef std::vector<ValueType> TypeVector;
typedef std::vector<AbstractSorter> SorterPtr;
typedef std::vector<AbstractFilter> FilterPtr;
typedef std::function<std::string(const char*)> DVNameSubstitutionFn;

typedef std::vector<
    DiscreteValue
> DiscreteValueVector;

typedef std::unordered_map<
    DiscreteHash,
    DiscreteValue*,
    DiscreteValueHasher,
    DiscreteValueComparer
> DiscreteHashMap;

typedef std::unordered_map<
    const char*,
    DiscreteValue*,
    DiscreteValueHasher,
    DiscreteValueComparer
> DiscreteNameMap;

typedef std::unordered_map<
    const char*,
    DiscreteValue,
    DiscreteValueHasher,
    DiscreteValueComparer
> DiscreteValuesByName;

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

class dataset_error : public std::logic_error {
public:
    dataset_error(const std::string& what) : std::logic_error(what) {}
};

} // namespace DataSet
} // namespace Vizzu
#endif //DATAFRAME_TYPES_H
