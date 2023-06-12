#ifndef TABLE_H
#define TABLE_H

#include <stdexcept>
#include <string>
#include <vector>

#include "columnindex.h"

namespace Vizzu
{
namespace Data
{

template <typename T> class TableRow
{
public:
	typedef std::vector<T> Values;

	TableRow() {}
	TableRow(size_t size) : values(size) {}
	TableRow(Values values) : values(std::move(values)) {}

	const T &operator[](const ColumnIndex &index) const
	{
		if (index >= values.size())
			throw std::logic_error("col index out of range");
		return values[index];
	}

	T &operator[](const ColumnIndex &index)
	{
		if (index >= values.size())
			throw std::logic_error("col index out of range");
		return values[index];
	}

	size_t size() const { return values.size(); }
	void pushBack(const T &value) { values.push_back(value); }

private:
	Values values;
};

template <typename T> struct Table
{
public:
	typedef std::vector<std::string> Header;
	typedef TableRow<T> Row;
	typedef std::vector<Row> Rows;

	size_t getColumnCount() const { return header.size(); }
	size_t getRowCount() const { return rows.size(); }

	const Row &operator[](size_t index) const
	{
		if (index >= rows.size())
			throw std::logic_error("row index out of range");
		return rows[index];
	}

	Row &operator[](size_t index)
	{
		if (index >= rows.size())
			throw std::logic_error("row index out of range");
		return rows[index];
	}

	const Header &getHeader() const { return header; }

	typename Rows::const_iterator begin() const
	{
		return rows.begin();
	}
	typename Rows::const_iterator end() const { return rows.end(); }

protected:
	Header header;
	Rows rows;

	void addHeader(const Header &header)
	{
		if (header.empty()) throw std::logic_error("empty header");
		this->header = header;
	}

	void addRow(const Row &row)
	{
		if (row.size() != getColumnCount())
			throw std::logic_error(
			    "row size missmatch, line "
			    + std::to_string(getRowCount() + 1) + ", expected "
			    + std::to_string(getColumnCount()) + " cells, got "
			    + std::to_string(row.size()));
		rows.push_back(row);
	}
};

}
}

#endif
