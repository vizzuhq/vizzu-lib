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
	void toggleItem(const Marker &item, bool add = true);
	Data::DataFilter getSelection();
	void setSelection(const Data::DataFilter &filter);
	void copySelectionFrom(const Diagram &source);
	bool anySelected();
	void toggleItems(const Data::MultiDim::SubSliceIndex &index);
	bool anySelected(const Data::MultiDim::SubSliceIndex &index) const;
	bool allSelected(const Data::MultiDim::SubSliceIndex &index) const;
	bool onlySelected(const Data::MultiDim::SubSliceIndex &index) const;
	void setSelection(const Data::MultiDim::SubSliceIndex &index, bool selected);
	void andSelection(const Data::MultiDim::SubSliceIndex &index);
private:
	Diagram &diagram;
	Data::DataFilter getEnableds();
};

}
}

#endif
