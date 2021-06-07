#include "selector.h"

using namespace Vizzu;
using namespace Vizzu::Diag;

Selector::Selector(Diagram &diagram)
	: diagram(diagram)
{}

void Selector::clearSelection()
{
	diagram.anySelected = false;
	for (auto &item: diagram.items) item.selected = false;
}

void Selector::toggleItem(const DiagramItem &item, bool add)
{
	auto alreadySelected = item.selected;

	if (!add) clearSelection();

	const_cast<Math::FuzzyBool&>(item.selected) = !alreadySelected;

	diagram.anySelected = anySelected();
}

Data::DataFilter Selector::getSelection()
{
	if (!anySelected()) return Data::DataFilter();

	Data::DataFilter filter;
	for(auto &item : diagram.items)
		if (item.enabled && item.selected)
			filter.addConditions(diagram.getDataCube().toFilterConditions(item.index));
	return filter;
}

Data::DataFilter Selector::getEnableds()
{
	Data::DataFilter filter;
	for(auto &item : diagram.items)
		if (item.enabled)
			filter.addConditions(diagram.getDataCube().toFilterConditions(item.index));
	return filter;
}


void Selector::setSelection(const Data::DataFilter &filter)
{
	for(auto &item : diagram.items)
	{
		if (item.enabled)
		{
			auto conditions = diagram.getDataCube().toFilterConditions(item.index);
			if (filter.match(conditions))
				item.selected = true;
		}
		else item.selected = false;
	}
	diagram.anySelected = anySelected();
}

void Selector::copySelectionFrom(const Diagram &source)
{
	if (source.getDataCube().empty()) return;

	Selector sourceSelector(const_cast<Diagram &>(source));

	if (source.anySelected) {
		auto sourceSelection = sourceSelector.getSelection();
		setSelection(sourceSelection);
	}
	else {
		clearSelection();
	}
}

bool Selector::anySelected()
{
	auto selectedCnt = 0u;
	auto allCnt = 0u;
	for (const auto &item: diagram.getItems()) {
		if (item.enabled) {
			if (item.selected) selectedCnt++;
			allCnt++;
		}
	}
	if (selectedCnt == allCnt) {
		clearSelection();
		selectedCnt = 0;
	}
	return selectedCnt > 0;
}

void Selector::toggleItems(const Data::MultiDim::SubSliceIndex &index)
{
	if (   ((double)diagram.anySelected == 0)
		|| !anySelected(index))
	{
		setSelection(index, true);
	}
	else if (allSelected(index) || onlySelected(index))
	{
		setSelection(index, false);
	}
	else andSelection(index);
}

bool Selector::anySelected(const Data::MultiDim::SubSliceIndex &index) const
{
	for (const auto &item: diagram.getItems())
		if (item.enabled && item.selected && index.contains(item.index))
			return true;
	return false;
}

bool Selector::allSelected(const Data::MultiDim::SubSliceIndex &index) const
{
	for (const auto &item: diagram.getItems())
		if (item.enabled && index.contains(item.index))
			if(!item.selected) return false;
	return true;
}

bool Selector::onlySelected(const Data::MultiDim::SubSliceIndex &index) const
{
	for (const auto &item: diagram.getItems())
		if (item.enabled && item.selected && !index.contains(item.index))
			return false;
	return true;
}

void Selector::setSelection(const Data::MultiDim::SubSliceIndex &index, bool selected)
{
	for (auto &item: diagram.items)
		if (item.enabled && index.contains(item.index))
	{
		item.selected = selected;
	}
	diagram.anySelected = anySelected();
}

void Selector::andSelection(const Data::MultiDim::SubSliceIndex &index)
{
	for (auto &item: diagram.items)
		if (item.enabled && item.selected)
	{
		item.selected = index.contains(item.index);
	}
	diagram.anySelected = anySelected();
}
