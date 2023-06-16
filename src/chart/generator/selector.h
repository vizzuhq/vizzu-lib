#ifndef SELECTOR_H
#define SELECTOR_H

#include "data/datacube/datafilter.h"
#include "data/table/datatable.h"

#include "plot.h"

namespace Vizzu
{
namespace Gen
{

class Selector
{
public:
	Selector(Plot &plot);

	void clearSelection();
	void toggleMarker(Marker &marker, bool add = true);
	bool anySelected();
	void toggleMarkers(const Data::MultiDim::SubSliceIndex &index);
	bool anySelected(
	    const Data::MultiDim::SubSliceIndex &index) const;
	bool allSelected(
	    const Data::MultiDim::SubSliceIndex &index) const;
	bool onlySelected(
	    const Data::MultiDim::SubSliceIndex &index) const;
	void setSelection(const Data::MultiDim::SubSliceIndex &index,
	    bool selected);
	void andSelection(const Data::MultiDim::SubSliceIndex &index);

private:
	Plot &plot;
};

}
}

#endif
