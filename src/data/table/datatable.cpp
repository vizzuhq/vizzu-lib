#include "datatable.h"

#include "base/io/log.h"
#include "texttable.h"

using namespace Vizzu;
using namespace Data;

DataTable::DataTable()
	: discreteSorting(SortType::Natural)
{}

DataTable::DataTable(const TextTable &table) :
    discreteSorting(SortType::Natural)
{
	processHeader(table);
	fillData(table);
	if (discreteSorting == SortType::Natural) sortDiscretes();
}

void DataTable::processHeader(const TextTable &table)
{
	header = table.getHeader();

	for (auto i = 0u; i < header.size(); i++)
	{
		auto &name = header[i];
		infos.emplace_back(name, table.getTypes()[i]);
		indexByName.insert({ name, ColumnIndex(i) });
	}
}

void DataTable::fillData(const TextTable &table)
{
	for (auto i = 0u; i < table.getRowCount(); i++)
		pushRow(table[i]);
}

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
	TextType::Type type;
	if constexpr (std::is_same_v<T, double>)
		type = TextType::Type::Number;
	else
		type = TextType::Type::String;

	size_t colIndex;

	auto count = values.size();

	auto it = indexByName.find(name);

	if (it == indexByName.end())
	{
		header.push_back(name);
		colIndex = header.size() - 1;
		indexByName.insert({name, ColumnIndex(colIndex)});
		infos.emplace_back(name, TextType(type, count));
	}
	else 
	{
		colIndex = it->second;
		infos[colIndex] = ColumnInfo(name, TextType(type, count));
	}

	for (auto i = 0u; i < getRowCount(); i++)
	{
		auto &row = rows.at(i);
		auto value = i < values.size() ? values[i] : T();
		if (colIndex < row.size())
			row[ColumnIndex(colIndex)] = infos[colIndex].registerValue(value);
		else
			row.pushBack(infos[colIndex].registerValue(value));
	}
	for (auto i = getRowCount(); i < values.size(); i++)
	{
		Row row;
		for (auto j = 0u; j < getColumnCount(); j++)
		{
			if (j == colIndex)
			{
				auto value = i < values.size() ? values[i] : T();
				row.pushBack(infos[j].registerValue(value));
			}
			else
				row.pushBack(infos[j].registerValue(std::string()));
		}

		addRow(row);
	}
	// todo: remove empty rows at the end?

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

void DataTable::sortDiscretes()
{
	Infos newInfos;
	std::vector<ColumnInfoConvert> converters;
	for (const auto &info: infos) {
		auto newInfo = info;
		newInfo.sort();
		newInfos.push_back(newInfo);
		converters.emplace_back(info, newInfo);
	}
	infos = std::move(newInfos);

	for (auto &row : rows)
	{
		for (auto j = 0u; j < row.size(); j++)
		{
			ColumnIndex col(j);
			if (infos[j].getType() == ColumnInfo::Discrete)
				row[col] = converters[j].convertValueIndex(row[col]);
		}
	}
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
