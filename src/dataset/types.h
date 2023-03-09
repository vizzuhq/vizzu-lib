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
namespace Dataset
{

const uint32_t nullSeriesId = 0;
const int nullpos = -1;
const int nullid = -1;

typedef double ContinousValue;
typedef uint64_t ValueId;
typedef uint32_t SeriesId;
typedef uint32_t TableId;
typedef uint64_t RecordId;
typedef uint32_t DiscreteHash;

class Value;
class DiscreteValue;
class DiscreteValueContainer;

class Range;
class AbstractSeries;
class Series;
class MutableSeries;

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

class AbstractTableGenerator;
class AbstractSeriesAggregator;
class AbstractSeriesGenerator;

namespace Generators {
class Ordinal;
};

namespace Aggregators {
class Count;
class Min;
class Max;
class Avarage;
};

class RecordAggregator;
class SeriesContainer;
class TableContainer;
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

typedef std::shared_ptr<AbstractSeries> SeriesPtr;
typedef std::shared_ptr<const Range> RangePtr;
typedef std::shared_ptr<const AbstractSeries> ConstSeriesPtr;
typedef std::shared_ptr<MutableSeries> MutableSeriesPtr;
typedef std::shared_ptr<SeriesIndex> SeriesIndexPtr;
typedef std::shared_ptr<Table> TablePtr;
typedef std::shared_ptr<const Table> ConstTablePtr;
typedef std::shared_ptr<AbstractTableGenerator> TableGeneratorPtr;
typedef std::vector<Value> ValueVector;
typedef std::vector<ValueType> TypeVector;
typedef std::vector<AbstractSorter> SorterPtr;
typedef std::vector<AbstractFilter> FilterPtr;
typedef std::function<std::string(const char *)> DVNameSubstitutionFn;
typedef std::vector<DiscreteValue> DiscreteValueVector;
typedef std::function<double(const MutableSeriesPtr&, const char*)> DiscreteToContinousConverterFn;
typedef std::function<std::string(const MutableSeriesPtr&, double)> ContinousToDiscreteConverterFn;

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

template<class S>
class IndexBasedIterator;

using ValueIterator = IndexBasedIterator<const AbstractSeries*>;

class dataset_error : public std::logic_error
{
public:
	dataset_error(const std::string &what) : std::logic_error(what) {}
};

} // namespace Dataset
} // namespace Vizzu
#endif // DATAFRAME_TYPES_H
