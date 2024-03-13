#ifndef DATATABLE_H
#define DATATABLE_H

#include <map>
#include <optional>
#include <span>
#include <string>

#include "columninfo.h"
#include "table.h"

namespace Vizzu::Data
{

enum class SortType : uint8_t { AsSeen, Natural };

class DataCube;

class DataTableOld : public Table<double>
{
	using Infos = std::vector<ColumnInfo>;

public:
	using Base = Table<double>;

	struct DataIndex
	{
		std::optional<ColumnIndex> value;
		ColumnInfo::Type type;

		DataIndex(ColumnIndex value, ColumnInfo::Type type) :
		    value(value),
		    type(type)
		{}
		DataIndex() : type(ColumnInfo::Type::measure) {}
		[[nodiscard]] bool isInvalid() const
		{
			return !value.has_value();
		}
	};

	DataTableOld();
	[[nodiscard]] const ColumnInfo &getInfo(ColumnIndex index) const;
	[[nodiscard]] DataIndex getIndex(ColumnIndex index) const;
	[[nodiscard]] ColumnIndex getColumn(
	    const std::string &name) const;
	[[nodiscard]] DataIndex getIndex(const std::string &name) const;

	DataIndex addColumn(const std::string &name,
	    const std::string &unit,
	    const std::span<const double> &values);
	DataIndex addColumn(const std::string &name,
	    const std::span<const char *const> &categories,
	    const std::span<const std::uint32_t> &values);

	void pushRow(const std::span<const char *> &cells);
	void pushRow(const TableRow<std::string> &textRow);

	[[nodiscard]] size_t columnCount() const;

	[[nodiscard]] std::string getInfos() const;

private:
	std::map<std::string, ColumnIndex> indexByName;
	Infos infos;

	template <typename T>
	DataIndex addTypedColumn(const std::string &name,
	    const std::string &unit,
	    const std::span<T> &values);
};

using DataTable = DataTableOld;

class CellWrapperOld
{
public:
	CellWrapperOld(const double &value, const ColumnInfo &info) :
	    value(value),
	    info(info)
	{}

	explicit operator std::string() const
	{
		return info.toString(value);
	}

	const double &operator*() const { return value; }

	double operator[](const std::string &val) const
	{
		return static_cast<double>(info.dimensionValueAt(val));
	}

	[[nodiscard]] const char *dimensionValue() const
	{
		return info.toDimensionString(value);
	}

	[[nodiscard]] bool isDimension() const
	{
		return info.getType() == ColumnInfo::Type::dimension;
	}

	[[nodiscard]] const ColumnInfo &getInfo() const { return info; }

private:
	const double &value;
	const ColumnInfo &info;
};

using CellWrapper = CellWrapperOld;

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
