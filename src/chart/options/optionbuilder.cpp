
#include "optionbuilder.h"

#include "data/table/datatable.h"

using namespace Vizzu;
using namespace Vizzu::Diag;

OptionsBuilder::OptionsBuilder(Options &options)
	: setter(options),
	  scaleId{ Scale::X, Scales::Index(0) }
{}

OptionsBuilder &OptionsBuilder::operator<<(const Data::DataTable &table)
{
	setter.setTable(&table);
	return *this;
}

OptionsBuilder &OptionsBuilder::operator<<(const Scale::Type &type)
{
	scaleId.type = type;
	scaleId.index = Scales::Index(0);
	return *this;
}

OptionsBuilder &OptionsBuilder::operator<<(const Scales::Index &index)
{
	scaleId.index = index;
	return *this;
}

OptionsBuilder &OptionsBuilder::operator<<(const ShapeType::Type &t)
{
	setter.setShape(t);
	return *this;
}

OptionsBuilder &OptionsBuilder::operator<<(const Base::Align::Type &alignType)
{
	setter.setAlign(alignType);
	return *this;
}

OptionsBuilder &OptionsBuilder::operator<<(const Polar &polar)
{
	setter.setPolar(polar.value);
	return *this;
}

OptionsBuilder &OptionsBuilder::operator<<(const Horizontal &horizontal)
{
	setter.setHorizontal(horizontal.value);
	return *this;
}

OptionsBuilder &OptionsBuilder::operator<<(const Data::SeriesIndex &index)
{
	setter.addSeries(scaleId, index);
	return *this;
}

OptionsBuilder &OptionsBuilder::operator<<(const char *seriesName)
{
	setter.addSeries(scaleId, seriesName);
	return *this;
}
