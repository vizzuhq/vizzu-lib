#include "seriesindex.h"

#include "base/text/funcstring.h"

using namespace Vizzu;
using namespace Vizzu::Data;


SeriesIndex::SeriesIndex(
	const SeriesType &type,
	const DataTable::DataIndex &dataIndex)
	: index(dataIndex.value), type(type)
{
	if (type.isReal())
	{
		if (dataIndex.isInvalid())
			throw std::logic_error
			("series type needs valid column index");

		if (	((dataIndex.type == ColumnInfo::Discrete) 
				&& !type.isNestedDiscrete())
			||	((dataIndex.type == ColumnInfo::Continous) 
				&& type.isNestedDiscrete())
		) throw std::logic_error
				("invalid series type for discrete column");
	}
}

SeriesIndex::SeriesIndex(const std::string &str, const DataTable &table)
{
	Text::FuncString func(str, false);
	if (!func.isEmpty())
	{
		auto type = Data::SeriesType::fromString(func.getName());

		const auto &params = func.getParams();
		if (params.size() == 0)
		{
			*this = SeriesIndex(type);
			return;
		}
		else if (params.size() == 1)
		{
			auto index = table.getIndex(params[0]);
			*this = SeriesIndex(type, index);
			return;
		}
		else throw std::logic_error
			("invalid data series type function");
	}
	else
	{
		set(table.getIndex(str));
	}
}

std::string SeriesIndex::toString(const DataTable &table) const {
	if (type.isReal()) {
		if (type.isContinous() && type != SeriesType::Sum)
			return type.toString() + "(" + table.getInfo(index).getName() + ")";
		else return table.getInfo(index).getName();
	} 
	else return type.toString() + "()";
}

std::string SeriesIndex::toString() const {
	if (type.isReal()) return std::to_string((size_t)index);
	else return type.toString();
}

void SeriesIndex::set(const DataTable::DataIndex &dataIndex)
{
	index = dataIndex.value;
	type = dataIndex.type == ColumnInfo::Discrete
			? SeriesType::Discrete : SeriesType::Sum;
}
