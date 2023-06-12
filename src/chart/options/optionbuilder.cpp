
#include "optionbuilder.h"

#include "data/table/datatable.h"

using namespace Vizzu;
using namespace Vizzu::Gen;

OptionsBuilder::OptionsBuilder(Options &options) :
    setter(options),
    scaleId(ScaleId::x)
{}

OptionsBuilder &OptionsBuilder::operator<<(
    const Data::DataTable &table)
{
	setter.setTable(&table);
	return *this;
}

OptionsBuilder &OptionsBuilder::operator<<(const ScaleId &type)
{
	scaleId = type;
	return *this;
}

OptionsBuilder &OptionsBuilder::operator<<(const ShapeType::Type &t)
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
	setter.addSeries(scaleId, index);
	return *this;
}

OptionsBuilder &OptionsBuilder::operator<<(const char *seriesName)
{
	setter.addSeries(scaleId, seriesName);
	return *this;
}
