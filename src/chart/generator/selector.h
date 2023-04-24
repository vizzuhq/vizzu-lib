#ifndef SELECTOR_H
#define SELECTOR_H

#include "diagram.h"
#include "data/datacube/datafilter.h"
#include "data/table/datatable.h"

namespace Vizzu
{
namespace Diag
{

class Selector
{
public:
	Selector(Diagram &diagram);

	void clearSelection();
	void toggleMarker(const Marker &marker, bool add = true);
	bool anySelected();
	void toggleMarkers(const Data::MultiDim::SubSliceIndex &index);
	bool anySelected(const Data::MultiDim::SubSliceIndex &index) const;
	bool allSelected(const Data::MultiDim::SubSliceIndex &index) const;
	bool onlySelected(const Data::MultiDim::SubSliceIndex &index) const;
	void setSelection(const Data::MultiDim::SubSliceIndex &index, bool selected);
	void andSelection(const Data::MultiDim::SubSliceIndex &index);
private:
	Diagram &diagram;
};

}
}

#endif
