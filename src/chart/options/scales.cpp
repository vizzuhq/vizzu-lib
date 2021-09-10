#include "scales.h"

using namespace Vizzu;
using namespace Vizzu::Diag;

Scales::Scales()
{
	for (auto type = 0u; type < ScaleId::EnumInfo::count(); type++)
		scales[(ScaleId)type] = Scale::makeScale(ScaleId(type));

	reset();
}

bool Scales::anyAxisSet() const
{
	return !scales[ScaleId::x].isEmpty() 
		|| !scales[ScaleId::y].isEmpty();
}

bool Scales::oneAxisSet() const
{
	return anyAxisSet() && !bothAxisSet();
}

bool Scales::bothAxisSet() const
{
	return !scales[ScaleId::x].isEmpty() 
		&& !scales[ScaleId::y].isEmpty();
}

bool Scales::isEmpty() const
{
	for (const auto &scale : scales)
		if(!scale.isEmpty()) return false;
	return true;
}

Data::DataCubeOptions::IndexSet Scales::getDimensions() const
{
	Data::DataCubeOptions::IndexSet dimensions;

	for (const auto &scale : scales)
		scale.collectDimesions(dimensions);

	return dimensions;
}

Data::DataCubeOptions::IndexSet Scales::getSeries() const
{
	Data::DataCubeOptions::IndexSet series;

	for (const auto &scale : scales)
		if (scale.continousId()) 
	{
		const auto &index = *scale.continousId();
		series.insert(index);
	}

	return series;
}

Data::DataCubeOptions::IndexSet Scales::getDimensions(const std::vector<ScaleId> &scaleTypes) const
{
	Data::DataCubeOptions::IndexSet dimensions;
	for (auto scaleType : scaleTypes)
		scales[(ScaleId)scaleType].collectDimesions(dimensions);
	return dimensions;
}

Data::DataCubeOptions::IndexSet Scales::getRealSeries(const std::vector<ScaleId> &scaleTypes) const
{
	Data::DataCubeOptions::IndexSet series;
	for (auto scaleType : scaleTypes)
		scales[(ScaleId)scaleType].collectRealSeries(series);
	return series;
}

Data::DataCubeOptions Scales::getDataCubeOptions() const
{
	return Data::DataCubeOptions(getDimensions(), getSeries());
}

std::pair<bool, Scale::OptionalIndex>
Scales::addSeries(
	const ScaleId &id,
	const Data::SeriesIndex &index,
	std::optional<size_t> pos)
{
	return scales[id].addSeries(index, pos);
}

bool Scales::removeSeries(const ScaleId &id, const Data::SeriesIndex &index)
{
	return scales[id].removeSeries(index);
}

bool Scales::clearSeries(const ScaleId &id)
{
	auto &scale = scales[id];
	if (scale.isEmpty()) return false;
	scales[id].reset();
	return true;
}

bool Scales::isSeriesUsed(const Data::SeriesIndex &index) const
{
	for (const auto &scale : scales)
		if (scale.isSeriesUsed(index))
			return true;
	return false;
}

bool Scales::isSeriesUsed(const std::vector<ScaleId> &scaleTypes, const Data::SeriesIndex &index) const
{
	for (auto scaleType : scaleTypes)
		if(scales[(ScaleId)scaleType].isSeriesUsed(index)) return true;
	return false;
}

size_t Scales::count(const Data::SeriesIndex &index) const
{
	size_t cnt = 0;
	for (const auto &scale : scales)
		if (scale.isSeriesUsed(index)) cnt++;
	return cnt;
}

std::list<ScaleId> Scales::find(const Data::SeriesIndex &index) const
{
	std::list<ScaleId> res;
	for (auto type = 0u; type < ScaleId::EnumInfo::count(); type++)
	{
		if (scales[(ScaleId)type].isSeriesUsed(index))
			res.push_back((ScaleId)type);
	}
	return res;
}

std::list<Scales::Pos> Scales::findPos(const Data::SeriesIndex &index) const
{
	std::list<Scales::Pos> res;
	for (auto type = 0u; type < ScaleId::EnumInfo::count(); type++)
	{
		auto pos = scales[(ScaleId)type].findPos(index);
		if (pos >= 0) res.push_back({ (ScaleId)type, pos });
	}
	return res;
}

const Scale &Scales::at(const ScaleId &id) const
{
	return scales.at(id);
}

Scale &Scales::at(const ScaleId &id)
{
	return scales.at(id);
}

ScaleId Scales::getEmptyAxisId() const
{
	if (at(ScaleId::x).isEmpty()) return ScaleId::x;
	if (at(ScaleId::y).isEmpty()) return ScaleId::y;
	return ScaleId(ScaleId::EnumInfo::count());
}

void Scales::reset()
{
	for (auto &scale: scales) scale.reset();
}

bool Scales::operator==(const Scales &other) const
{
	return scales == other.scales;
}

void Scales::visitAll(
	const std::function<void(ScaleId, const Scale &)> &visitor) const
{
	for (auto type = 0u; type < ScaleId::EnumInfo::count(); type++)
	{
		const auto &scale = scales[(ScaleId)type];
		visitor((ScaleId)type, std::ref(scale));
	}
}
