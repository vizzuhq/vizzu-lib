#ifndef SERIESINDEX_H
#define SERIESINDEX_H

#include <stdexcept>
#include <list>

#include "base/type/uniquelist.h"
#include "seriestype.h"

namespace Vizzu
{
namespace Data
{

class SeriesIndex
{
public:

	SeriesIndex() {}

	SeriesIndex(const SeriesType &type,
				const DataTable::DataIndex &dataIndex = DataTable::DataIndex());

	SeriesIndex(const DataTable::DataIndex &dataIndex)
		: index(dataIndex.value),
		  type(dataIndex.type == ColumnInfo::Discrete
			   ? SeriesType::Discrete : SeriesType::Sum)
	{
		set(dataIndex);
	}

	SeriesIndex(const std::string &str, const DataTable &table);

	ColumnIndex getColIndex() const { return index; }
	SeriesType getType() const { return type; }

	bool operator<(const SeriesIndex &other) const
	{ return index < other.index || (index == other.index && type < other.type); }

	bool operator==(const SeriesIndex &other) const
	{ return index == other.index && type == other.type; }

	std::string toString(const DataTable &table) const;
	std::string toString() const;

private:
	ColumnIndex index;
	SeriesType type;
	void set(const DataTable::DataIndex &dataIndex);
};

typedef Type::UniqueList<SeriesIndex> SeriesList;

}
}

#endif

