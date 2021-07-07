#include "datatable.h"

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
	{
		auto inRow = table[i];
		Row row;
		for (auto j = 0u; j < table.getColumnCount(); j++)
		{
			row.pushBack(infos[j].registerValue(inRow[ColumnIndex(j)]));
		}
		addRow(row);
	}
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

	auto count = values.size();
	auto colIndex = header.size() - 1;

	TextType::Type type;
	if constexpr (std::is_same_v<T, double>)
		type = TextType::Type::Number;
	else
		type = TextType::Type::String;

	infos.emplace_back(name, TextType(type, count));
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
