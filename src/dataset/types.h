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

const int nullpos = -1;
const int nullid = -1;
typedef uint64_t DatasetId;
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

struct ConstCharPtrComparator {
    bool operator()(const char* a, const char* b) const;
};

struct DiscreteValueComparer
{
	bool operator()(const char *, const char *) const;
	bool operator()(const DiscreteValue &,
	    const DiscreteValue &) const;
};

struct DiscreteValueHasher
{
	size_t operator()(const char *) const;
	size_t operator()(const DiscreteValue &) const;
};

using ConstantTablePtr = std::shared_ptr<const AbstractConstantTable>;
using TableContainer = std::map<const char*, ConstantTablePtr, ConstCharPtrComparator>;
using ConstantSeriesPtr = std::shared_ptr<AbstractConstantSeries>;
using RangePtr = std::shared_ptr<Range>;
using RangeContainer = std::map<std::string, RangePtr>;
using SeriesItem = struct {ConstantSeriesPtr series; RangePtr range;};
using SeriesContainer = std::map<const char*, SeriesItem, ConstCharPtrComparator>;
using VolatileSeriesPtr = std::shared_ptr<AbstractVolatileSeries>;
enum class ValueType { null, discrete, continous };

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

typedef std::shared_ptr<AbstractTableGenerator> TableGeneratorPtr;
typedef std::vector<Value> ValueVector;
typedef std::vector<ValueType> TypeVector;
typedef std::shared_ptr<AbstractSorter> SorterPtr;
typedef std::shared_ptr<AbstractFilter> FilterPtr;
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

template<class S>
class IndexBasedIterator;

using ValueIterator = IndexBasedIterator<const AbstractConstantSeries*>;
using RangeValueIterator = IndexBasedIterator<const Range*>;
class RangeIndexIterator;

class dataset_error : public std::logic_error
{
public:
	dataset_error(const std::string &what) : std::logic_error(what) {}
};

} // namespace Dataset
} // namespace Vizzu
#endif // DATAFRAME_TYPES_H
