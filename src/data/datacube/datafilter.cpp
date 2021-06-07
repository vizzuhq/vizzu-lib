#include "datafilter.h"

#include <unordered_map>

#include "datacube.h"

using namespace Vizzu;
using namespace Vizzu::Data;

bool DataFilter::hasConditions(const DataFilter::AndConditions &andConditions) const
{
	return (find(andConditions) != orConditions.end());
}

bool DataFilter::addCondition(int orIndex,
    const Condition &condition)
{
	if ((int)orConditions.size() < orIndex)
		throw std::logic_error("invalid datafilter 'or' index: "
		                       + std::to_string(orIndex));

	if ((int)orConditions.size() <= orIndex)
		orConditions.push_back(Data::DataFilter::AndConditions());

	auto &orcs = orConditions[orIndex];

	orcs.push_back(condition);

	return true;
}

bool DataFilter::addConditions(const DataFilter::AndConditions &andConditions)
{
	if (!hasConditions(andConditions))
	{
		orConditions.push_back(andConditions);
		return true;
	}
	else return false;
}

bool DataFilter::operator&=(const DataFilter &filter)
{
	auto changed = false;
	for (const auto &conditions: filter.orConditions)
		changed |= addConditions(conditions);
	return changed;
}

bool DataFilter::removeConditions(const DataFilter::AndConditions &andConditions)
{
	auto it = find(andConditions);

	if (it != orConditions.end())
	{
		orConditions.erase(it);
		return true;
	}
	else return false;
}

bool DataFilter::removeCondition(const DataFilter::Condition &condition)
{
	bool res = false;
	for (auto &andConditions: orConditions)
	{
		auto size = andConditions.size();
		std::remove(andConditions.begin(), andConditions.end(), condition);
		if (size != andConditions.size()) res = true;
	}
	return res;
}

bool DataFilter::match(const TableRow<double> &row) const
{
	if (orConditions.empty()) return true;

	for (const auto &orCondition: orConditions)
	{
		bool allEq = true;
		for (const auto &andCondition : orCondition)
		{
			if (row[andCondition.index] != andCondition.value) {
				allEq = false;
				break;
			}
		}
		if (allEq) return true;
	}
	return false;
}

bool DataFilter::match(const AndConditions &andConditions) const
{
	if (orConditions.empty()) return true;

	std::unordered_map<ColumnIndex::UnderlyingType, MultiDim::Index> andMap;
	for (const auto &cond : andConditions)
		andMap[cond.index] = cond.value;

	for (const auto &orCondition : orConditions)
	{
		bool allEq = true;
		for (const auto &andCondition : orCondition)
		{
			if (andMap[andCondition.index] != andCondition.value) {
				allEq = false;
				break;
			}
		}
		if (allEq) return true;
	}
	return false;
}

bool DataFilter::clear()
{
	if (orConditions.empty()) return false;
	orConditions.clear();
	return true;
}

bool DataFilter::isEmpty() const
{
	return orConditions.empty();
}

DataFilter DataFilter::operator+(const DataFilter &other) const
{
	DataFilter res(*this);
	res.orConditions.insert(
				orConditions.end(),
				other.orConditions.begin(),
				other.orConditions.end());
	return res;
}

std::string DataFilter::toString(const DataTable &table) const
{
	std::string res;
	if (orConditions.empty()) res = "all";
	else for (const auto &andConditions: orConditions)
	{
		std::string s;

		if (andConditions.empty()) s = "all";
		else for (const auto &condition : andConditions)
		{
			s += (!s.empty() ? " AND " : "") + condition.toString(table);
		}
		res += std::string(!res.empty() ? "\nOR " : "") + "(" + s + ")";
	}
	return "(" + res + ")";
}

DataFilter::OrConditions::const_iterator
DataFilter::find(const AndConditions &andConditions) const
{
	return std::find(orConditions.begin(),
					 orConditions.end(),
					 andConditions);
}

DataFilterStack::DataFilterStack()
{
	pushNew();
}

const DataFilter &DataFilterStack::top() const
{
	return filters.back();
}

DataFilter &DataFilterStack::top()
{
	return filters.back();
}

DataFilterStack::DataFilterList::const_iterator
DataFilterStack::begin() const
{
	return filters.begin();
}

DataFilterStack::DataFilterList::iterator
DataFilterStack::begin()
{
	return filters.begin();
}

DataFilterStack::DataFilterList::const_iterator
DataFilterStack::end() const
{
	return filters.end();
}

DataFilterStack::DataFilterList::iterator
DataFilterStack::end()
{
	return filters.end();
}

bool DataFilterStack::pushNew()
{
	if (filters.empty() || !filters.back().isEmpty()) {
		filters.emplace_back();
		return true;
	}
	else return false;
}

bool DataFilterStack::match(const TableRow<double> &row) const
{
	for (const auto &filter: filters)
		if (!filter.match(row)) return false;
	return true;
}

bool DataFilterStack::operator==(const DataFilterStack &other) const
{
	return filters == other.filters;
}

std::string DataFilterStack::toString(const DataTable &table) const
{
	std::string res;
	if (filters.empty()) res = "all";
	else for (const auto &filter: filters)
		res += (!res.empty() ? "\nAND " : "") + filter.toString(table);
	return "(" + res + ")";
}

std::string DataFilter::Condition::toString(const DataTable &table) const
{
	return "(" + table.getInfo(index).getName()
			+ " == "
			+ table.getInfo(index).discreteValues().at(value) + ")";
}

DataFilter::Condition::Condition(const DataTable &table,
    const std::string &index,
    const std::string &value)
{
	this->index = table.getColumn(index);
	this->value = Data::MultiDim::Index
	{
		table.getInfo(this->index).discreteValueIndexes().at(value)
	};
}
