#ifndef DATAFRAME_TYPES_H
#define DATAFRAME_TYPES_H

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

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
class AbstractSeriesGenerator;
class OrdNumSeries;
class SeriesContainer;

class SeriesIndex;
class AbstractFilter;
class AbstractSorter;
class SingleColumnSorter;
class MultiColumnSorter;

class Cell;
class Row;
class RowContainer;
class Column;
class ColumnContainer;
class Table;

class AbstractTableBuilder;
class AbstractSeriesAggregator;
class Identical;
class Aggregator;
class Normalizer;
class Pivoter;

class TableContainer;
class DataSet;

struct DiscreteValueComparer
{
	bool operator()(const char *, const char *) const;
	bool operator()(const DiscreteValue &,
	    const DiscreteValue &) const;
};

enum class ValueType { null, discrete, continous };

struct DiscreteValueHasher
{
	size_t operator()(const char *) const;
	size_t operator()(const DiscreteValue &) const;
};

typedef std::shared_ptr<AbstractSeries> SeriesPtr;
typedef std::shared_ptr<const AbstractSeries> ConstSeriesPtr;
typedef std::shared_ptr<MutableSeries> MutableSeriesPtr;
typedef std::shared_ptr<SeriesIndex> SeriesIndexPtr;
typedef std::shared_ptr<Table> TablePtr;
typedef std::shared_ptr<const Table> ConstTablePtr;
typedef std::shared_ptr<AbstractTableBuilder> TableBuilderPtr;
typedef std::vector<Value> ValueVector;
typedef std::vector<ValueType> TypeVector;
typedef std::vector<AbstractSorter> SorterPtr;
typedef std::vector<AbstractFilter> FilterPtr;
typedef std::function<std::string(const char *)> DVNameSubstitutionFn;
typedef std::vector<DiscreteValue> DiscreteValueVector;
typedef std::function<double(const MutableSeriesPtr&, const char*)> DiscreteToContinousNormalizerFn;
typedef std::function<std::string(const MutableSeriesPtr&, double)> ContinousToDiscreteNormalizerFn;

typedef std::unordered_map<
    DiscreteHash,
    DiscreteValue *,
    DiscreteValueHasher,
    DiscreteValueComparer>
    DiscreteHashMap;

typedef std::unordered_map<
    const char *,
    DiscreteValue *,
    DiscreteValueHasher,
    DiscreteValueComparer>
    DiscreteNameMap;

typedef std::unordered_map<
    const char *,
    DiscreteValue,
    DiscreteValueHasher,
    DiscreteValueComparer>
    DiscreteValuesByName;

typedef std::unordered_set<
    DiscreteValue,
    DiscreteValueHasher,
    DiscreteValueComparer>
    DiscreteValueSet;

typedef std::unordered_map<SeriesId, SeriesPtr, std::hash<SeriesId>> SeriesMap;
typedef SeriesMap::const_iterator SeriesIterator;
typedef std::unordered_map<TableId, TablePtr, std::hash<TableId>>TableMap;
typedef TableMap::iterator TableIterator;
typedef std::unordered_map<std::string, std::string> SeriesInfo;
typedef std::vector<SeriesPtr> SeriesVector;
typedef std::vector<int> IndexVector;

class dataset_error : public std::logic_error
{
public:
	dataset_error(const std::string &what) : std::logic_error(what) {}
};

} // namespace DataSet
} // namespace Vizzu
#endif // DATAFRAME_TYPES_H
