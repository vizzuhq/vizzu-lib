#ifndef DATAFRAME_TYPES_H
#define DATAFRAME_TYPES_H

#include <string.h>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <map>
#include <span>

namespace Vizzu
{
namespace Dataset
{

namespace ParamNames {
	constexpr const char* disp_name = "name";
	constexpr const char* description = "desc";
}

const int nullpos = -1;
const int nullid = -1;
typedef uint64_t DatasetId;
typedef uint32_t DiscreteHash;

class AbstractConstantSeries;
class AbstractVolatileSeries;
class AbstractConstantTable;
class AbstractVolatileTable;
class AbstractFilter;
class AbstractTableSorter;
class AbstractTableGenerator;
class AbstractSeriesAggregator;
class AbstractSeriesGenerator;

class DiscreteValue;
class Value;
class Dataset;
class Series;
class RawSeries;
class Range;
class Cell;
class Row;
class Column;
class Table;
class ColumnContainer;
class RowContainer;
class DiscreteValueContainer;
class RecordAggregator;

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

using ContinousValue = double;
using ConstantTablePtr = std::shared_ptr<const AbstractConstantTable>;
using TableContainer = std::map<const char*, ConstantTablePtr, ConstCharPtrComparator>;
using ConstantSeriesPtr = std::shared_ptr<AbstractConstantSeries>;
using RangePtr = std::shared_ptr<Range>;
using SeriesContainer = std::map<const char*, ConstantSeriesPtr, ConstCharPtrComparator>;
using RangeContainer = std::map<const char*, RangePtr, ConstCharPtrComparator>;
using VolatileSeriesPtr = std::shared_ptr<AbstractVolatileSeries>;
enum class ValueType { null, discrete, continous };
using TableGeneratorPtr = std::shared_ptr<AbstractTableGenerator>;
using ValueVector = std::vector<Value>;
using TypeVector = std::vector<ValueType>;
using IdVector = std::vector<DatasetId>;
using SorterPtr = std::shared_ptr<AbstractTableSorter>;
using FilterPtr = std::shared_ptr<AbstractFilter>;
using DVNameSubstitutionFn = std::function<std::string(const char *)>;
using ValueConverter = std::function<Value(Dataset&, ValueType, const Value&)>;
enum class TableSeriesType { unfiltered, unsorted, final };
using ParameterMap = std::map<std::string, std::string>;

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
