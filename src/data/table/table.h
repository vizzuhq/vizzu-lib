#ifndef TABLE_H
#define TABLE_H

#include <stdexcept>
#include <string>
#include <vector>

#include "columnindex.h"

namespace Vizzu::Data
{

template <typename T> class TableRow
{
public:
	using Values = std::vector<T>;

	TableRow() = default;
	explicit TableRow(size_t size) : values(size) {}
	explicit TableRow(Values values) : values(std::move(values)) {}

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

	[[nodiscard]] size_t size() const { return values.size(); }
	void pushBack(const T &value) { values.push_back(value); }

private:
	Values values;
};

template <typename T> struct Table
{
public:
	using Header = std::vector<std::string>;
	using Row = TableRow<T>;
	using Rows = std::vector<Row>;

	[[nodiscard]] size_t getColumnCount() const
	{
		return header.size();
	}
	[[nodiscard]] size_t getRowCount() const { return rows.size(); }

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

	[[nodiscard]] const Header &getHeader() const { return header; }

	[[nodiscard]] typename Rows::const_iterator begin() const
	{
		return rows.begin();
	}
	[[nodiscard]] typename Rows::const_iterator end() const
	{
		return rows.end();
	}

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
			    "row size mismatch, line "
			    + std::to_string(getRowCount() + 1) + ", expected "
			    + std::to_string(getColumnCount()) + " cells, got "
			    + std::to_string(row.size()));
		rows.push_back(row);
	}
};

}

#endif
