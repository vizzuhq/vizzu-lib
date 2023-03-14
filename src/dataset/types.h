#ifndef DATAFRAME_TYPES_H
#define DATAFRAME_TYPES_H

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <map>

namespace Vizzu
{
namespace Dataset
{

const uint32_t nullSeriesId = 0;
const int nullpos = -1;
const int nullid = -1;

typedef uint64_t ValueId;
typedef uint32_t SeriesId;
typedef uint32_t TableId;
typedef uint64_t RecordId;
typedef uint32_t DiscreteHash;

class AbstractConstantSeries;
class AbstractVolatileSeries;
class AbstractConstantTable;
class AbstractFilter;
class AbstractSorter;
class AbstractTableGenerator;
class AbstractSeriesAggregator;
class AbstractSeriesGenerator;

class DiscreteValue;
using ContinousValue = double;
class Value;
class Series;
class OriginalSeries;
class ProducedSeries;
class Range;

using ConstantTablePtr = std::shared_ptr<AbstractConstantTable>;
using TableContainer = std::map<const char*, ConstantTablePtr>;
using ConstantSeriesPtr = std::shared_ptr<AbstractConstantSeries>;
using RangePtr = std::shared_ptr<Range>;
using RangeContainer = std::map<std::string, RangePtr>;
using SeriesItem = struct {ConstantSeriesPtr series; RangePtr range;};
using SeriesContainer = std::map<const char*, SeriesItem>;
using VolatileSeriesPtr = std::shared_ptr<AbstractVolatileSeries>;

class Cell;
class Row;
class Column;
class Table;

class ColumnContainer;
class RowContainer;
class DiscreteValueContainer;
class SeriesIndex;

class RecordAggregator;
class Dataset;

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

typedef std::shared_ptr<Table> TablePtr;
typedef std::shared_ptr<const Table> ConstTablePtr;
typedef std::shared_ptr<AbstractTableGenerator> TableGeneratorPtr;
typedef std::vector<Value> ValueVector;
typedef std::vector<ValueType> TypeVector;
typedef std::shared_ptr<AbstractSorter> TableSorterPtr;
typedef std::shared_ptr<AbstractFilter> TableFilterPtr;
typedef std::function<std::string(const char *)> DVNameSubstitutionFn;
typedef std::vector<DiscreteValue> DiscreteValueVector;
typedef std::function<double(const AbstractConstantSeries&, const char*)> DiscreteToContinousConverter;
typedef std::function<std::string(const AbstractConstantSeries&, double)> ContinousToDiscreteConverter;

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

typedef std::unordered_map<TableId, TablePtr, std::hash<TableId>>TableMap;
typedef TableMap::iterator TableIterator;
typedef std::unordered_map<std::string, std::string> SeriesInfo;

template<class S>
class IndexBasedIterator;

using ValueIterator = IndexBasedIterator<const AbstractConstantSeries*>;

class dataset_error : public std::logic_error
{
public:
	dataset_error(const std::string &what) : std::logic_error(what) {}
};

} // namespace Dataset
} // namespace Vizzu
#endif // DATAFRAME_TYPES_H
