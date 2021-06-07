#ifndef HIERARCHY_H
#define HIERARCHY_H

#include <unordered_map>
#include <list>
#include <optional>
#include "columnindex.h"

namespace Vizzu
{
namespace Data
{

class Hierarchy
{
public:
	void setParentChild(const ColumnIndex &parent, const ColumnIndex &child);
	bool isChild(const ColumnIndex &index) const;
	std::optional<ColumnIndex> childOf(const ColumnIndex &parent) const;
	std::optional<ColumnIndex> parentOf(const ColumnIndex &child) const;
	std::optional<ColumnIndex> baseParent(const ColumnIndex &index) const;
	bool empty() const;

private:
	typedef std::list<ColumnIndex> ChildList;
	typedef std::unordered_map<uint64_t, ChildList> Parents;
	struct ChildPos { Parents::iterator parent; ChildList::iterator pos; };
	typedef std::unordered_map<uint64_t, ChildPos> Children;

	Parents parents;
	Children children;
};

}
}

#endif
