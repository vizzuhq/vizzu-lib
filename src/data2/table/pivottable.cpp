#include "pivottable.h"

using namespace Vizzu;
using namespace Data;

Vizzu::Data::PivotTable::PivotTable(TextTable table, bool hasKeyColumn)
	: table(std::move(table)), hasKeyColumn(hasKeyColumn)
{}

TextTable PivotTable::getTable()
{
	TextTable result;

	if (table.getRowCount() < 1
		|| table.getColumnCount() < 1)
		return result;

	auto header = table.getHeader();

	std::vector<std::string> resultHeader;
	resultHeader.emplace_back("header");
	resultHeader.emplace_back("key");
	resultHeader.emplace_back("data");
	result.setHeader(resultHeader);

	for (auto row = 0u; row < table.getRowCount(); row++)
	{
		const auto &rowLine = table[row];

		auto colMin = hasKeyColumn ? 1u : 0u;

		for (auto col = colMin; col < table.getColumnCount(); col++)
		{
			if (col >= rowLine.size()) continue;

			auto key = hasKeyColumn
					   ? rowLine[Data::ColumnIndex{0}]
					   : std::to_string(row);

			std::vector<std::string> resultRow;
			resultRow.push_back(header[col]);
			resultRow.push_back("<>"+key);
			resultRow.push_back(rowLine[Data::ColumnIndex{col}]);
			result.pushBackRow(resultRow);
		}
	}
	return result;
}
