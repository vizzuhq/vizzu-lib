#ifndef DATAFILTER_H
#define DATAFILTER_H

#include <vector>
#include <list>

#include "data/table/columnindex.h"
#include "data/table/table.h"
#include "data/table/datatable.h"
#include "data/multidim/multidimindex.h"

namespace Vizzu
{
namespace Data
{

class DataCube;

class DataFilter
{
public:
	struct Condition
	{
		ColumnIndex index;
		MultiDim::Index value;

		Condition() = default;

		Condition(ColumnIndex index, MultiDim::Index value) :
			index(index), value(value)
		{}

		Condition(const DataTable &table,
		    const std::string &index,
		    const std::string &value);

		bool operator==(const Condition &other) const
		{
			return index == other.index && value == other.value;
		}

		std::string toString(const DataTable &table) const;
	};

	typedef std::vector<Condition> AndConditions;
	typedef std::vector<AndConditions> OrConditions;

	typedef OrConditions UnderlyingType;

	bool hasConditions(const AndConditions &andConditions) const;
	bool addConditions(const AndConditions &andConditions);
	bool addCondition(int orIndex, const Condition &condition);
	bool operator&=(const DataFilter &filter);
	bool removeConditions(const AndConditions &andConditions);
	bool removeCondition(const Condition &refCondition);
	bool match(const TableRow<double> &row) const;
	bool match(const AndConditions &andConditions) const;
	bool clear();
	bool isEmpty() const;

	bool operator==(const DataFilter &other) const {
		return orConditions == other.orConditions;
	}

	DataFilter operator+(const DataFilter &other) const;

	std::string toString(const DataTable &table) const;

private:

	OrConditions orConditions;

	OrConditions::const_iterator find(const AndConditions &andConditions) const;
};

class DataFilterStack
{
public:
	typedef std::list<DataFilter> DataFilterList;
	typedef DataFilterList UnderlyingType;

	DataFilterStack();
	const DataFilter &top() const;
	DataFilter &top();
	DataFilterList::const_iterator begin() const;
	DataFilterList::iterator begin();
	DataFilterList::const_iterator end() const;
	DataFilterList::iterator end();
	bool pushNew();
	bool match(const TableRow<double> &row) const;
	bool operator==(const DataFilterStack& other) const;
	std::string toString(const DataTable &table) const;
private:
	DataFilterList filters;
};

}
}

#endif
