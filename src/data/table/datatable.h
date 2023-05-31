#ifndef DATATABLE_H
#define DATATABLE_H

#include <map>
#include <string>
#include <span>
#include <list>

#include "table.h"
#include "columninfo.h"

namespace Vizzu
{
namespace Data
{

enum class SortType : uint8_t
{
	AsSeen,
	Natural
};

class DataCube;

class DataTable : public Table<double>
{
public:
	typedef Table<double> Base;

	struct DataIndex
	{
		ColumnIndex value;
		ColumnInfo::Type type;

		DataIndex(ColumnIndex value, ColumnInfo::Type type)
			: value(value), type(type) {}
		DataIndex() :
		    value((size_t)-1),
		    type(ColumnInfo::Type::Continous)
		{}
		bool isInvalid() const { return value == (size_t)-1; }
	};

	DataTable();
	const ColumnInfo &getInfo(ColumnIndex index) const;
	DataIndex getIndex(ColumnIndex index) const;
	ColumnIndex getColumn(const std::string &name) const;
	DataIndex getIndex(const std::string &name) const;

	DataIndex addColumn(const std::string &name,
	    const std::span<double> &values);
	DataIndex addColumn(const std::string &name,
	    const std::span<std::string> &values);
	DataIndex addColumn(const std::string &name,
	    const std::span<const char*> &values);

	void pushRow(const std::span<const char*> &cells);
	void pushRow(const TableRow<std::string> &textRow);

	size_t columnCount() const;

private:
	typedef std::vector<ColumnInfo> Infos;

	std::map<std::string, ColumnIndex> indexByName;
	Infos infos;

	template <typename T>
	DataIndex addTypedColumn(const std::string &name,
	    const std::span<T> &values);
};

class CellWrapper
{
public:
	CellWrapper(const double &value, const ColumnInfo &info) :
		value(value), info(info)
	{
	}

	explicit operator std::string() const {
		return info.toString(value);
	}

	const double &operator*() const { return value; }

	double operator[](const std::string &val) const
	{
		return info.discreteValueIndexes().at(val);
	}

	const char *discreteValue() const {
		return info.toDiscreteString(value);
	}

	const ColumnInfo &getInfo() const { return info; }

private:
	const double &value;
	const ColumnInfo &info;
};

class RowWrapper
{
public:
	RowWrapper(const DataTable &table, const DataTable::Row &row)
	: table(table), row(row)
	{}

	CellWrapper operator[](const std::string &columnName) const
	{
		auto colIndex = table.getColumn(columnName);
		return this->operator[](colIndex);
	}

	CellWrapper operator[](ColumnIndex colIndex) const
	{
		const auto &info = table.getInfo(colIndex);
		return CellWrapper(row[colIndex], info);
	}

	size_t size() const { return row.size(); }

private:
	const DataTable &table;
	const DataTable::Row &row;
};

}
}

#endif
