#ifndef OPTION_OPERATIONS_H
#define OPTION_OPERATIONS_H

#include "data/table/datatable.h"

#include "optionssetter.h"

namespace Vizzu
{
namespace Gen
{

class Operations
{
public:
	Operations(OptionsSetterPtr setter, const Data::DataTable &table);

	void addSeries(const Data::SeriesIndex &index);
	void removeSeries(const Data::SeriesIndex &index);
	void split();
	void stack();
	void fit(bool enable);
	void swapDimension();

	bool isFit() const;

protected:
	OptionsSetterPtr setter;
	const Data::DataTable &table;

private:
	void split(const Gen::ChannelId &mainId,
	    const Gen::ChannelId &subId);
	void stack(const Gen::ChannelId &mainId,
	    const Gen::ChannelId &subId);
};

}
}

#endif
