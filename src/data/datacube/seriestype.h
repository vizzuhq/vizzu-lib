#ifndef SERIESTYPE_H
#define SERIESTYPE_H
 
#include <string_view>
#include <initializer_list>

#include "data/table/datatable.h"

#include "aggregator.h"

namespace Vizzu
{
namespace Data
{

class SeriesType
{
	typedef ColumnInfo::Type CT;
	typedef Aggregator::Type AT;

public:

	static const SeriesType Index;
	static const SeriesType Discrete;
	static const SeriesType Exists;
	static const SeriesType Sum;
	static const SeriesType Count;
	static const SeriesType Distinct;
	static const SeriesType Min;
	static const SeriesType Max;
	static const SeriesType Mean;

	constexpr bool isDiscrete() const {
		return columnType == CT::Discrete;
	}

	constexpr bool isContinous() const {
		return columnType == CT::Continous;
	}

	constexpr bool isReal() const {
		return real;
	}

	constexpr bool isValid() const {
		return !name.empty();
	}

	constexpr ColumnInfo::Type getColumnType() {
		return columnType;
	}

	constexpr Aggregator::Type aggregatorType() const {
		return (Aggregator::Type)index;
	}

	constexpr SeriesType() = default;

	constexpr SeriesType(bool real, 
		ColumnInfo::Type columnType, 
		ColumnInfo::Type nestedColumnType, 
		size_t index, 
		const char * name)
		: real(real), columnType(columnType), 
		  nestedColumnType(nestedColumnType), 
		  index(index), name(name)
	{}

	constexpr bool operator==(const SeriesType &other) const {
		return index == other.index && columnType == other.columnType;
	}

	constexpr bool operator<(const SeriesType &other) const {
		return index < other.index
			|| (index == other.index && columnType < other.columnType);
	}

	std::string toString() const { return { name.data(), name.size() }; }
	static SeriesType fromString(std::string_view name, bool throws = true);

	constexpr bool isNestedDiscrete() const {
		return nestedColumnType == CT::Discrete;
	}

private:
	static const std::initializer_list<SeriesType> constTypes;

	bool real; // needs data series
	ColumnInfo::Type columnType;
	ColumnInfo::Type nestedColumnType;
	uint64_t index;
	std::string_view name;
};

}
}

#endif
