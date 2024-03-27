#include "datatable.h"

#include "base/conv/auto_json.h"

namespace Vizzu::Data
{

DataTableOld::DataTableOld() = default;

void DataTableOld::pushRow(const std::span<const char *> &cells)
{
	std::vector<std::string> strCells(cells.begin(), cells.end());
	pushRow(TableRow<std::string>(std::move(strCells)));
}

void DataTableOld::pushRow(const TableRow<std::string> &textRow)
{
	Row row;
	for (auto i = 0U; i < getColumnCount(); ++i) {
		if (i < textRow.size())
			row.pushBack(
			    infos[i].registerValue(textRow[ColumnIndex(i)]));
	}
	addRow(row);
}

template <typename T>
void DataTableOld::addTypedColumn(const std::string &name,
    const std::string &unit,
    const std::span<T> &values)
{
	TextType type{};
	if constexpr (std::is_same_v<std::remove_const_t<T>, double>)
		type = TextType::Number;
	else
		type = TextType::String;

	size_t colIndex{};

	auto it = indexByName.find(name);

	if (it == indexByName.end()) {
		header.push_back(name);
		colIndex = header.size() - 1;
		indexByName.insert({name, ColumnIndex(colIndex)});
		infos.emplace_back(name, unit, type);
	}
	else {
		colIndex = it->second;
		auto columnInfo = ColumnInfo(name, unit, type);
		if (columnInfo.getType() != infos[colIndex].getType())
			infos[colIndex] = columnInfo;
		else
			infos[colIndex].reset();
	}

	for (auto i = 0U; i < getRowCount(); ++i) {
		auto &row = rows.at(i);
		auto value = i < values.size() ? values[i] : T{};
		if constexpr (std::is_same_v<T, const char *>)
			if (value == nullptr) value = "";
		if (colIndex < row.size())
			row[ColumnIndex(colIndex)] =
			    infos[colIndex].registerValue(value);
		else
			row.pushBack(infos[colIndex].registerValue(value));
	}
	for (auto i = getRowCount(); i < values.size(); ++i) {
		Row row;
		for (auto j = 0U; j < getColumnCount(); ++j) {
			if (j == colIndex) {
				auto value = i < values.size() ? values[i] : T();
				if constexpr (std::is_same_v<T, const char *>)
					if (value == nullptr) value = "";
				row.pushBack(infos[j].registerValue(value));
			}
			else
				row.pushBack(infos[j].registerValue(std::string()));
		}

		addRow(row);
	}
}

void DataTableOld::addColumn(const std::string &name,
    const std::string &unit,
    const std::span<const double> &values)
{
	return addTypedColumn(name, unit, values);
}

void DataTableOld::addColumn(const std::string &name,
    const std::span<const char *const> &categories,
    const std::span<const std::uint32_t> &values)
{
	std::vector<const char *> realValues(values.size());
	for (auto i = 0U; i < values.size(); ++i)
		realValues[i] = categories[values[i]];

	return addTypedColumn<const char *>(name, {}, realValues);
}

const ColumnInfo &DataTableOld::getInfo(ColumnIndex index) const
{
	return infos[index];
}

DataTableOld::DataIndex DataTableOld::getIndex(
    ColumnIndex index) const
{
	return {index, infos[index].getType()};
}

ColumnIndex DataTableOld::getColumn(const std::string &name) const
{
	auto it = indexByName.find(name);
	if (it != indexByName.end()) return it->second;
	throw std::logic_error("No column name exists: " + name);
}

DataTableOld::DataIndex DataTableOld::getIndex(
    const std::string &name) const
{
	return getIndex(getColumn(name));
}

size_t DataTableOld::columnCount() const { return infos.size(); }

std::string DataTableOld::getInfos() const
{
	return Conv::toJSON(infos);
}

}