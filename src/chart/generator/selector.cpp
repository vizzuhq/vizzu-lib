#include "selector.h"

using namespace Vizzu;
using namespace Vizzu::Diag;

Selector::Selector(Diagram &diagram)
	: diagram(diagram)
{}

void Selector::clearSelection()
{
	diagram.anySelected = false;
	for (auto &marker: diagram.markers) marker.selected = false;
}

void Selector::toggleMarker(const Marker &marker, bool add)
{
	auto alreadySelected = marker.selected;

	if (!add) clearSelection();

	const_cast<Math::FuzzyBool&>(marker.selected) = !alreadySelected;

	diagram.anySelected = anySelected();
}

bool Selector::anySelected()
{
	auto selectedCnt = 0u;
	auto allCnt = 0u;
	for (const auto &marker: diagram.getMarkers()) {
		if (marker.enabled) {
			if (marker.selected) selectedCnt++;
			allCnt++;
		}
	}
	if (selectedCnt == allCnt) {
		clearSelection();
		selectedCnt = 0;
	}
	return selectedCnt > 0;
}

void Selector::toggleMarkers(const Data::MultiDim::SubSliceIndex &index)
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
	for (const auto &marker: diagram.getMarkers())
		if (marker.enabled && marker.selected && index.contains(marker.index))
			return true;
	return false;
}

bool Selector::allSelected(const Data::MultiDim::SubSliceIndex &index) const
{
	for (const auto &marker: diagram.getMarkers())
		if (marker.enabled && index.contains(marker.index))
			if(!marker.selected) return false;
	return true;
}

bool Selector::onlySelected(const Data::MultiDim::SubSliceIndex &index) const
{
	for (const auto &marker: diagram.getMarkers())
		if (marker.enabled && marker.selected && !index.contains(marker.index))
			return false;
	return true;
}

void Selector::setSelection(const Data::MultiDim::SubSliceIndex &index, bool selected)
{
	for (auto &marker: diagram.markers)
		if (marker.enabled && index.contains(marker.index))
	{
		marker.selected = selected;
	}
	diagram.anySelected = anySelected();
}

void Selector::andSelection(const Data::MultiDim::SubSliceIndex &index)
{
	for (auto &marker: diagram.markers)
		if (marker.enabled && marker.selected)
	{
		marker.selected = index.contains(marker.index);
	}
	diagram.anySelected = anySelected();
}
