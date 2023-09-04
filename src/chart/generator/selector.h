#ifndef SELECTOR_H
#define SELECTOR_H

#include "data/datacube/datafilter.h"
#include "data/table/datatable.h"

#include "plot.h"

namespace Vizzu::Gen
{

class Selector
{
public:
	explicit Selector(Plot &plot);

	void clearSelection();
	void toggleMarker(Marker &marker, bool add = true);
	bool anySelected();
	void toggleMarkers(const Data::MultiDim::SubSliceIndex &index);
	[[nodiscard]] bool anySelected(
	    const Data::MultiDim::SubSliceIndex &index) const;
	[[nodiscard]] bool allSelected(
	    const Data::MultiDim::SubSliceIndex &index) const;
	[[nodiscard]] bool onlySelected(
	    const Data::MultiDim::SubSliceIndex &index) const;
	void setSelection(const Data::MultiDim::SubSliceIndex &index,
	    bool selected);
	void andSelection(const Data::MultiDim::SubSliceIndex &index);

private:
	Plot &plot;
};

}

#endif
