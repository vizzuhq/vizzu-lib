#ifndef DATATABLE_H
#define DATATABLE_H

#include <dataframe/old/datatable.h>
#include <map>
#include <optional>
#include <span>
#include <string>

namespace Vizzu::Data
{

using DataTable = data_table;

using CellWrapper = DataTable::cell_wrapper;

class RowWrapper
{
public:
	RowWrapper(const DataTable &table, const DataTable::Row &row) :
	    table(table),
	    row(row)
	{}

	CellWrapper operator[](const std::string &columnName) const
	{
		auto colIndex = table.getColumn(columnName);
		return {row[colIndex], table.getInfo(colIndex)};
	}

	[[nodiscard]] size_t size() const { return row.size(); }

private:
	const DataTable &table;
	const DataTable::Row &row;
};

}

#endif
