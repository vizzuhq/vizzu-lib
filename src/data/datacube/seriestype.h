#ifndef SERIESTYPE_H
#define SERIESTYPE_H

#include <initializer_list>
#include <string_view>

#include "data/table/datatable.h"

#include "aggregator.h"

namespace Vizzu::Data
{

class SeriesType
{
	using CT = ColumnInfo::Type;
	using AT = Aggregator::Type;

public:
	static const SeriesType Index;
	static const SeriesType Dimension;
	static const SeriesType Exists;
	static const SeriesType Sum;
	static const SeriesType Count;
	static const SeriesType Distinct;
	static const SeriesType Min;
	static const SeriesType Max;
	static const SeriesType Mean;

	[[nodiscard]] constexpr bool isDimension() const
	{
		return columnType == CT::dimension;
	}

	[[nodiscard]] constexpr bool isMeasure() const
	{
		return columnType == CT::measure;
	}

	[[nodiscard]] constexpr bool isReal() const { return real; }

	[[nodiscard]] constexpr bool isValid() const
	{
		return !name.empty();
	}

	constexpr ColumnInfo::Type getColumnType() { return columnType; }

	[[nodiscard]] constexpr Aggregator::Type aggregatorType() const
	{
		return static_cast<Aggregator::Type>(index);
	}

	constexpr SeriesType() = default;

	constexpr SeriesType(bool real,
	    ColumnInfo::Type columnType,
	    ColumnInfo::Type nestedColumnType,
	    size_t index,
	    const char *name) :
	    real(real),
	    columnType(columnType),
	    nestedColumnType(nestedColumnType),
	    index(index),
	    name(name)
	{}

	constexpr bool operator==(const SeriesType &other) const
	{
		return index == other.index && columnType == other.columnType;
	}

	constexpr bool operator<(const SeriesType &other) const
	{
		return index < other.index
		    || (index == other.index
		        && columnType < other.columnType);
	}

	[[nodiscard]] std::string toString() const
	{
		return {name.data(), name.size()};
	}
	static SeriesType fromString(std::string_view name,
	    bool throws = true);

	[[nodiscard]] constexpr bool isNestedDimension() const
	{
		return nestedColumnType == CT::dimension;
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

#endif
