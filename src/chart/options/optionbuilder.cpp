
#include "optionbuilder.h"

#include "data/table/datatable.h"

using namespace Vizzu;
using namespace Vizzu::Gen;

OptionsBuilder::OptionsBuilder(Options &options) :
    setter(options),
    channelId(ChannelId::x)
{}

OptionsBuilder &OptionsBuilder::operator<<(
    const Data::DataTable &table)
{
	setter.setTable(&table);
	return *this;
}

OptionsBuilder &OptionsBuilder::operator<<(const ChannelId &type)
{
	channelId = type;
	return *this;
}

OptionsBuilder &OptionsBuilder::operator<<(const ShapeType &t)
{
	setter.setShape(t);
	return *this;
}

OptionsBuilder &OptionsBuilder::operator<<(
    const Base::Align::Type &alignType)
{
	setter.setAlign(alignType);
	return *this;
}

OptionsBuilder &OptionsBuilder::operator<<(const Polar &polar)
{
	setter.setPolar(polar.value);
	return *this;
}

OptionsBuilder &OptionsBuilder::operator<<(
    const Horizontal &horizontal)
{
	setter.setHorizontal(horizontal.value);
	return *this;
}

OptionsBuilder &OptionsBuilder::operator<<(
    const Data::SeriesIndex &index)
{
	setter.addSeries(channelId, index);
	return *this;
}

OptionsBuilder &OptionsBuilder::operator<<(const char *seriesName)
{
	setter.addSeries(channelId, seriesName);
	return *this;
}
