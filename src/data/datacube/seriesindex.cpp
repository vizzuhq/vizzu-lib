#include "seriesindex.h"

#include "base/conv/tostring.h"
#include "base/text/funcstring.h"

namespace Vizzu::Data
{

SeriesIndex::SeriesIndex(const SeriesType &type,
    const DataTable::DataIndex *dataIndex) :
    index(dataIndex ? dataIndex->value : std::nullopt),
    type(type)
{
	if (type.isReal()) {
		if (!dataIndex || dataIndex->isInvalid())
			throw std::logic_error(
			    "series type needs valid column index");

		if (((dataIndex->type
		         == DataTable::DataIndex::Type::dimension)
		        && !type.isNestedDimension())
		    || ((dataIndex->type
		            == DataTable::DataIndex::Type::measure)
		        && type.isNestedDimension()))
			throw std::logic_error(
			    "invalid series type for dimension column");
	}
}

SeriesIndex::SeriesIndex(const DataTable::DataIndex &dataIndex) :
    SeriesIndex(dataIndex.type
                        == DataTable::DataIndex::Type::dimension
                    ? SeriesType::Dimension
                    : SeriesType::Sum,
        &dataIndex)
{}

SeriesIndex::SeriesIndex(const std::string &str,
    const DataTable &table)
{
	const Text::FuncString func(str, false);
	if (const Data::SeriesType type{
	        func.isEmpty()
	            ? Data::SeriesType()
	            : Data::SeriesType::fromString(func.getName(),
	                false)};
	    type.isValid()) {
		const auto &params = func.getParams();
		if (params.empty()) {
			*this = SeriesIndex(type);
			return;
		}
		if (params.size() == 1) {
			auto index = table.getIndex(params[0]);
			*this = SeriesIndex(type, &index);
			return;
		}
		throw std::logic_error("invalid data series type function");
	}

	set(table.getIndex(str));
}

std::string SeriesIndex::toString(const DataTable &table) const
{
	if (type.isReal()) {
		if (type.isMeasure() && type != SeriesType::Sum)
			return type.toString() + "("
			     + table.getInfo(index.value()).getName() + ")";

		return table.getInfo(index.value()).getName();
	}
	return type.toString() + "()";
}

void SeriesIndex::set(const DataTable::DataIndex &dataIndex)
{
	index = dataIndex.value;
	type = dataIndex.type == DataTable::DataIndex::Type::dimension
	         ? SeriesType::Dimension
	         : SeriesType::Sum;
}

}