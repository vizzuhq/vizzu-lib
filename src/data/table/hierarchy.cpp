#include "hierarchy.h"

#include <algorithm>
#include <stdexcept>

using namespace Vizzu;
using namespace Vizzu::Data;

void Hierarchy::setParentChild(const ColumnIndex &parent, const ColumnIndex &child)
{
	auto isChildParent = parents.find((size_t)child);
	if (isChildParent != parents.end())
	{
		throw std::logic_error("cannont set parent series as child");
	}

	auto childIt = children.find((size_t)child);
	if (childIt != children.end())
	{
		childIt->second.parent->second.erase(childIt->second.pos);
		if (childIt->second.parent->second.empty())
			parents.erase(childIt->second.parent);
	}

	auto isParentChild = children.find((size_t)parent);
	if (isParentChild != children.end())
	{
		auto targetPos = isParentChild->second.pos;
		targetPos++;
		auto pos = isParentChild->second.parent->second.insert(
						targetPos, child);

		children.insert({ (size_t)child, { isParentChild->second.parent,
										   pos } });
	}
	else
	{
		auto parentIt = parents.find((size_t)parent);
		if (parentIt == parents.end())
			parentIt = parents.insert({ (size_t)parent, ChildList() }).first;

		parentIt->second.push_front(child);
		children.insert({ (size_t)child, { parentIt, parentIt->second.begin() } });
	}
}

bool Hierarchy::isChild(const ColumnIndex &index) const
{
	return children.find((size_t)index) != children.end();
}

std::optional<ColumnIndex> Hierarchy::childOf(const ColumnIndex &parent) const
{
	auto parentIt = parents.find((size_t)parent);
	if (parentIt != parents.end())
		return *parentIt->second.begin();

	auto childIt = children.find((size_t)parent);
	if (childIt != children.end()) {
		auto it = childIt->second.pos;
		++it;
		if (it != childIt->second.parent->second.end())
			return *it;
		else
			return std::nullopt;
	}
	return std::nullopt;
}

std::optional<ColumnIndex> Hierarchy::parentOf(const ColumnIndex &child) const
{
	auto childIt = children.find((size_t)child);
	if (childIt != children.end()) {
		auto it = childIt->second.pos;
		if (it != childIt->second.parent->second.begin()) {
			--it;
			return *it;
		} else {
			return ColumnIndex(childIt->second.parent->first);
		}
	}
	return std::nullopt;
}

std::optional<ColumnIndex> Hierarchy::baseParent(const ColumnIndex &index) const
{
	auto parentIt = parents.find((size_t)index);
	if (parentIt != parents.end())
		return index;

	auto childIt = children.find((size_t)index);
	if (childIt != children.end())
		return ColumnIndex(childIt->second.parent->first);

	return std::nullopt;
}

bool Hierarchy::empty() const
{
	return parents.empty();
}

