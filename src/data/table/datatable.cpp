#include "datatable.h"

using namespace Vizzu;
using namespace Data;

DataTable::DataTable() = default;

void DataTable::pushRow(const std::span<const char *> &cells)
{
	std::vector<std::string> strCells(cells.begin(), cells.end());
	pushRow(TableRow<std::string>(std::move(strCells)));
}

void DataTable::pushRow(const TableRow<std::string> &textRow)
{
	Row row;
	for (auto i = 0U; i < getColumnCount(); i++) {
		if (i < textRow.size())
			row.pushBack(
			    infos[i].registerValue(textRow[ColumnIndex(i)]));
	}
	addRow(row);
}

template <typename T>
DataTable::DataIndex DataTable::addTypedColumn(
    const std::string &name,
    const std::span<T> &values)
{
	TextType type;
	if constexpr (std::is_same_v<T, double>)
		type = TextType::Number;
	else
		type = TextType::String;

	size_t colIndex;

	auto it = indexByName.find(name);

	if (it == indexByName.end()) {
		header.push_back(name);
		colIndex = header.size() - 1;
		indexByName.insert({name, ColumnIndex(colIndex)});
		infos.emplace_back(name, type);
	}
	else {
		colIndex = it->second;
		auto columnInfo = ColumnInfo(name, type);
		if (columnInfo.getType() != infos[colIndex].getType())
			infos[colIndex] = columnInfo;
		else
			infos[colIndex].reset();
	}

	for (auto i = 0U; i < getRowCount(); i++) {
		auto &row = rows.at(i);
		auto value = i < values.size() ? values[i] : T{};
		if constexpr (std::is_same_v<T, const char*>)
			if (value == nullptr)
				value = "";
		if (colIndex < row.size())
			row[ColumnIndex(colIndex)] =
			    infos[colIndex].registerValue(value);
		else
			row.pushBack(infos[colIndex].registerValue(value));
	}
	for (auto i = getRowCount(); i < values.size(); i++) {
		Row row;
		for (auto j = 0U; j < getColumnCount(); j++) {
			if (j == colIndex) {
				auto value = i < values.size() ? values[i] : T();
				if constexpr (std::is_same_v<T, const char*>)
					if (value == nullptr)
						value = "";
				row.pushBack(infos[j].registerValue(value));
			}
			else
				row.pushBack(infos[j].registerValue(std::string()));
		}

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
	return {index, infos[index].getType()};
}

ColumnIndex DataTable::getColumn(const std::string &name) const
{
	auto it = indexByName.find(name);
	if (it != indexByName.end())
		return it->second;
	else
		throw std::logic_error("No column name exists: " + name);
}

DataTable::DataIndex DataTable::getIndex(
    const std::string &name) const
{
	return getIndex(getColumn(name));
}

size_t DataTable::columnCount() const { return infos.size(); }
