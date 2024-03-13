#ifndef SERIESINDEX_H
#define SERIESINDEX_H

#include <optional>
#include <stdexcept>

#include "base/type/uniquelist.h"

#include "seriestype.h"

namespace Vizzu::Data
{

class SeriesIndex
{
public:
	using OptColIndex = std::remove_reference_t<
	    decltype(DataTable::DataIndex::value)>;

	SeriesIndex() = default;
	explicit SeriesIndex(const SeriesType &type,
	    const DataTable::DataIndex &dataIndex =
	        DataTable::DataIndex());
	explicit SeriesIndex(const DataTable::DataIndex &dataIndex);
	SeriesIndex(const std::string &str, const DataTable &table);

	[[nodiscard]] OptColIndex getColIndex() const { return index; }
	[[nodiscard]] SeriesType getType() const { return type; }

	bool operator<(const SeriesIndex &other) const
	{
		return index < other.index
		    || (index == other.index && type < other.type);
	}

	bool operator==(const SeriesIndex &other) const = default;

	[[nodiscard]] std::string toString(const DataTable &table) const;
	[[nodiscard]] std::string toString() const;

private:
	OptColIndex index;
	SeriesType type;
	void set(const DataTable::DataIndex &dataIndex);
};

using SeriesList = Type::UniqueList<SeriesIndex>;

}

#endif
