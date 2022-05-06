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
				const DataTable::DataIndex &dataIndex = DataTable::DataIndex())
		: index(dataIndex.value), type(type)
	{
		if (type.isReal())
		{
			if (dataIndex.isInvalid())
				throw std::logic_error
				("internal error: series type needs valid column index");

			if (	((dataIndex.type == ColumnInfo::Discrete) && !type.isDiscrete())
				||	((dataIndex.type == ColumnInfo::Continous) && type.isDiscrete())
			) throw std::logic_error
					("internal error: invalid series type for discrete column");
		}
	}

	SeriesIndex(const DataTable::DataIndex &dataIndex)
		: index(dataIndex.value),
		  type(dataIndex.type == ColumnInfo::Discrete
			   ? SeriesType::Discrete : SeriesType::Sum)
	{
		set(dataIndex);
	}

	SeriesIndex(const std::string &str, const DataTable &table)
	{
		try {
			index = DataTable::DataIndex().value;
			type = SeriesType::fromString(str);
			return;
		}
		catch(std::exception &) {}

		set(table.getIndex(str));
	}

	void set(const DataTable::DataIndex &dataIndex)
	{
		index = dataIndex.value;
		type = dataIndex.type == ColumnInfo::Discrete
			   ? SeriesType::Discrete : SeriesType::Sum;
	}

	ColumnIndex getColIndex() const { return index; }
	SeriesType getType() const { return type; }

	bool operator<(const SeriesIndex &other) const
	{ return index < other.index || (index == other.index && type < other.type); }

	bool operator==(const SeriesIndex &other) const
	{ return index == other.index && type == other.type; }

	std::string toString(const DataTable &table) const {
		if (type.isReal()) return table.getInfo(index).getName();
		else return type.toString();
	}

	std::string toString() const {
		if (type.isReal()) return std::to_string((size_t)index);
		else return type.toString();
	}

private:
	ColumnIndex index;
	SeriesType type;
};

typedef Type::UniqueList<SeriesIndex> SeriesList;

}
}

#endif

