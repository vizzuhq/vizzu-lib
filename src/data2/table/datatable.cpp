#include "datatable.h"

#include "base/io/log.h"

using namespace Vizzu;
using namespace Data;

DataTable::DataTable()
{}

void DataTable::pushRow(const std::span<const char*> &cells)
{
	std::vector<std::string> strCells(cells.begin(), cells.end());
	pushRow(TableRow<std::string>(std::move(strCells)));
}

void DataTable::pushRow(const TableRow<std::string> &textRow)
{
	Row row;
	for (auto i = 0u; i < getColumnCount(); i++)
	{
		if (i < textRow.size())
			row.pushBack(infos[i].registerValue(textRow[ColumnIndex(i)]));
	}
	addRow(row);
}

template <typename T>
DataTable::DataIndex DataTable::addTypedColumn(
    const std::string &name,
    const std::span<T> &values)
{
	auto it = indexByName.find(name);
	if (it != indexByName.end())
		throw std::logic_error("Column name already exists: " + name);

	header.push_back(name);
	auto colIndex = header.size() - 1;

	TextType type;
	if constexpr (std::is_same_v<T, double>)
		type = TextType::Number;
	else
		type = TextType::String;

	infos.emplace_back(name, type);
	indexByName.insert({name, ColumnIndex(colIndex)});

	for (auto i = 0u; i < getRowCount(); i++)
	{
		auto &row = rows.at(i);
		auto value = i < values.size() ? values[i] : T();
		row.pushBack(infos.at(colIndex).registerValue(value));
	}
	for (auto i = getRowCount(); i < values.size(); i++)
	{
		Row row;
		for (auto j = 0u; j < colIndex; j++)
			row.pushBack(infos[j].registerValue(std::string()));

		auto value = i < values.size() ? values[i] : T();
		row.pushBack(infos.at(colIndex).registerValue(value));

		addRow(row);
	}
	return getIndex(ColumnIndex(colIndex));
}

DataTable::DataIndex DataTable::addColumn(const std::string &name,
    const std::span<double> &values)
{
	return addTypedColumn(name, values);
}

DataTable::DataIndex DataTable::addColumn(const std::string &name,
    const std::span<const char *> &values)
{
	return addTypedColumn(name, values);
}

DataTable::DataIndex DataTable::addColumn(const std::string &name,
    const std::span<std::string> &values)
{
	return addTypedColumn(name, values);
}

const ColumnInfo &DataTable::getInfo(ColumnIndex index) const
{
	return infos[index];
}

DataTable::DataIndex DataTable::getIndex(ColumnIndex index) const
{
	return { index, infos[index].getType() };
}

ColumnIndex DataTable::getColumn(const std::string &name) const
{
	auto it = indexByName.find(name);
	if (it != indexByName.end())
		return it->second;
	else
		throw std::logic_error("No column name exists: " + name);
}

DataTable::DataIndex DataTable::getIndex(const std::string &name) const
{
	return getIndex(getColumn(name));
}
