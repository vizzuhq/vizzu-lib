#include "selector.h"

Vizzu::Gen::Selector::Selector(Plot &plot) : plot(plot) {}

void Vizzu::Gen::Selector::clearSelection()
{
	plot.anySelected = false;
	for (auto &marker : plot.markers) marker.selected = false;
}

void Vizzu::Gen::Selector::toggleMarker(Marker &marker, bool add)
{
	auto alreadySelected = marker.selected;

	if (!add) clearSelection();

	marker.selected = !alreadySelected;

	plot.anySelected = anySelected();
}

bool Vizzu::Gen::Selector::anySelected()
{
	auto selectedCnt = 0U;
	auto allCnt = 0U;
	for (const auto &marker : plot.getMarkers()) {
		if (static_cast<double>(marker.enabled) > 0) {
			if (static_cast<double>(marker.selected) > 0)
				selectedCnt++;
			allCnt++;
		}
	}
	if (selectedCnt == allCnt) {
		clearSelection();
		selectedCnt = 0;
	}
	return selectedCnt > 0;
}

void Vizzu::Gen::Selector::toggleMarkers(
    const Data::MultiDim::SubSliceIndex &index)
{
	if ((static_cast<double>(plot.anySelected) == 0)
	    || !anySelected(index)) {
		setSelection(index, true);
	}
	else if (allSelected(index) || onlySelected(index)) {
		setSelection(index, false);
	}
	else
		andSelection(index);
}

bool Vizzu::Gen::Selector::anySelected(
    const Data::MultiDim::SubSliceIndex &index) const
{
	return std::ranges::any_of(plot.getMarkers(),
	    [&](const auto &marker)
	    {
		    return marker.enabled && marker.selected
		        && index.contains(marker.index);
	    });
}

bool Vizzu::Gen::Selector::allSelected(
    const Data::MultiDim::SubSliceIndex &index) const
{
	return std::ranges::all_of(plot.getMarkers(),
	    [&](const auto &marker)
	    {
		    return !marker.enabled || !index.contains(marker.index)
		        || marker.selected;
	    });
}

bool Vizzu::Gen::Selector::onlySelected(
    const Data::MultiDim::SubSliceIndex &index) const
{
	return std::ranges::all_of(plot.getMarkers(),
	    [&](const auto &marker)
	    {
		    return !marker.enabled || !marker.selected
		        || index.contains(marker.index);
	    });
}

void Vizzu::Gen::Selector::setSelection(
    const Data::MultiDim::SubSliceIndex &index,
    bool selected)
{
	for (auto &marker : plot.markers)
		if (marker.enabled && index.contains(marker.index)) {
			marker.selected = selected;
		}
	plot.anySelected = anySelected();
}

void Vizzu::Gen::Selector::andSelection(
    const Data::MultiDim::SubSliceIndex &index)
{
	for (auto &marker : plot.markers)
		if (marker.enabled && marker.selected) {
			marker.selected = index.contains(marker.index);
		}
	plot.anySelected = anySelected();
}
