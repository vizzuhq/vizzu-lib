#include "scales.h"

using namespace Vizzu;
using namespace Vizzu::Diag;

std::string Scales::Id::toString() const
{
	return  "{ " + std::to_string((size_t)index) + ", " + Diag::toString(type) + " }";
}

Scales::Scales()
{
	reset();
}

bool Scales::anyAxisSet() const
{
	return anyScaleOfType(Scale::X) || anyScaleOfType(Scale::Y);
}

bool Scales::oneAxisSet() const
{
	return anyAxisSet() && !bothAxisSet();
}

bool Scales::bothAxisSet() const
{
	return anyScaleOfType(Scale::X) && anyScaleOfType(Scale::Y);
}

bool Scales::isEmpty() const
{
	for (const auto &scaleVect : scales)
		for (const auto &scale : scaleVect)
			if(!scale.isEmpty()) return false;
	return true;
}

Scales::Index Scales::maxScaleSize() const
{
	size_t res = 0u;
	for (const auto &scaleVect : scales)
		res = std::max(res, scaleVect.size());
	return Scales::Index(res);
}

bool Scales::anyScaleOfType(Scale::Type type) const
{
	for (const auto &scale : scales[type])
		if (!scale.isEmpty()) return true;
	return false;
}

Data::DataCubeOptions::IndexSet Scales::getDimensions() const
{
	Data::DataCubeOptions::IndexSet dimensions;

	for (const auto &scaleVect : scales)
		for (const auto &scale : scaleVect)
			scale.collectDimesions(dimensions);

	return dimensions;
}

Data::DataCubeOptions::IndexSet Scales::getSeries() const
{
	Data::DataCubeOptions::IndexSet series;

	for (const auto &scaleVect : scales)
		for (const auto &scale : scaleVect)
			if (scale.continousId()) {
				const auto &index = *scale.continousId();
				series.insert(index);
			}

	return series;
}

Data::DataCubeOptions::IndexSet Scales::getDimensions(const std::vector<Scale::Type> &scaleTypes) const
{
	Data::DataCubeOptions::IndexSet dimensions;
	for (auto scaleType : scaleTypes)
		for (const auto &scale : scales[(Scale::Type)scaleType])
			scale.collectDimesions(dimensions);
	return dimensions;
}

Data::DataCubeOptions::IndexSet Scales::getRealSeries(const std::vector<Scale::Type> &scaleTypes) const
{
	Data::DataCubeOptions::IndexSet series;
	for (auto scaleType : scaleTypes)
		for (const auto &scale : scales[(Scale::Type)scaleType])
			scale.collectRealSeries(series);
	return series;
}

Data::DataCubeOptions Scales::getDataCubeOptions() const
{
	return Data::DataCubeOptions(getDimensions(), getSeries());
}

const Scales::ScaleList &Scales::operator[](Scale::Type type) const
{
	return scales[type];
}

std::pair<bool, Scale::OptionalContinousIndex>
Scales::addSeries(const Scales::Id &id,
					   const Data::SeriesIndex &index,
					   std::optional<size_t> pos)
{
	return getScale(id).addSeries(index, pos);
}

bool Scales::removeSeries(const Scales::Id &id, const Data::SeriesIndex &index)
{
	auto res = getScale(id).removeSeries(index);

	auto &scaleVect = scales[id.type];

	int lastUsedIndex = (int)scaleVect.size() - 1;
	while (lastUsedIndex >= 0 && scaleVect[lastUsedIndex].isEmpty())
		lastUsedIndex--;
	scaleVect.resize(lastUsedIndex + 1);

	return res;
}

bool Scales::clearSeries(const Id &id)
{
	auto &scale = getScale(id);
	if (scale.isEmpty()) return false;
	getScale(id).reset();
	return true;
}

bool Scales::isSeriesUsed(const Data::SeriesIndex &index) const
{
	for (const auto &scaleVect : scales)
		for (const auto &scale : scaleVect)
			if (scale.isSeriesUsed(index))
				return true;
	return false;
}

bool Scales::isSeriesUsed(const std::vector<Scale::Type> &scaleTypes, const Data::SeriesIndex &index) const
{
	bool used = false;
	for (auto scaleType : scaleTypes)
		for (const auto &scale : scales[(Scale::Type)scaleType])
			if(scale.isSeriesUsed(index)) used = true;
	return used;
}

size_t Scales::count(const Data::SeriesIndex &index) const
{
	size_t cnt = 0;
	for (const auto &scaleVect : scales)
		for (const auto &scale : scaleVect)
			if (scale.isSeriesUsed(index)) cnt++;
	return cnt;
}

std::list<Scales::Id> Scales::find(const Data::SeriesIndex &index) const
{
	std::list<Scales::Id> res;
	visitAll([&](Scales::Id id, const Scale& scale)
	{
		if (scale.isSeriesUsed(index)) res.push_back(id);
	});
	return res;
}

std::list<Scales::Pos> Scales::findPos(const Data::SeriesIndex &index) const
{
	std::list<Scales::Pos> res;
	visitAll([&](Scales::Id id, const Scale& scale)
	{
		auto pos = scale.findPos(index);
		if (pos >= 0) res.push_back({ id, pos });
	});
	return res;
}

std::array<Scale, Scale::Type::id_size>
Scales::makeNulls() const
{
	std::array<Scale, Scale::Type::id_size> res;
	for (auto i = 0u; i < res.size(); i++)
		res[i] = Scale::makeScale(Scale::Type(i));
	return res;
}

Scale &Scales::getScale(const Scales::Id &id)
{
	auto &scaleVect = scales[id.type];
	if (id.index >= scaleVect.size())
		scaleVect.resize(id.index + 1, Scale::makeScale(id.type));
	return scaleVect[id.index];
}

const Scale &Scales::at(const Scales::Id &id, bool top) const
{
	return at(id.type, id.index, top);
}

Scale &Scales::at(const Scales::Id &id, bool top)
{
	return at(id.type, id.index, top);
}

const Scale &Scales::at(const Scale::Type &type,
						const Scales::Index &index,
						bool top) const
{
	static const std::array<Scale, Scale::Type::id_size> nulls = makeNulls();

	const auto &scaleVect = scales[type];

	if (scaleVect.empty()
		|| index >= maxScaleSize()
		|| (!top && index >= scaleVect.size()))
		return nulls[type];

	return scaleVect[std::min((size_t)index, scaleVect.size()-1)];
}

Scale &Scales::at(const Scale::Type &type,
						const Scales::Index &index,
						bool top)
{
	static std::array<Scale, Scale::Type::id_size> nulls = makeNulls();

	auto &scaleVect = scales.at(type);

	if (scaleVect.empty()
		|| index >= maxScaleSize()
		|| (!top && index >= scaleVect.size()))
		return nulls.at(type);

	return scaleVect[std::min((size_t)index, scaleVect.size()-1)];
}

Scales::Id Scales::getEmptyAxisId() const
{
	if (at(Scale::X).isEmpty()) return Scale::X;
	if (at(Scale::Y).isEmpty()) return Scale::Y;
	return Scale::Type::id_size;
}

Scales::Level Scales::getScales(Scales::Index index) const
{
	std::array<const Scale *, Scale::id_size>  scales;
	for (auto i = 0u; i < scales.size(); i++)
		scales[i] = &at(Scales::Id{ Scale::Type(i), index });
	return scales;
}

void Scales::reset()
{
	for (auto &scale: scales) scale.clear();
}

bool Scales::operator==(const Scales &other) const
{
	return scales == other.scales;
}

void Scales::visitAll(const std::function<void(Id, const Scale &)> &visitor) const
{
	for (auto type = 0u; type < (size_t)Scale::Type::id_size; type++)
	{
		const auto &scaleVector = scales[(Scale::Type)type];
		for (auto index = 0u; index < scaleVector.size(); index++)
		{
			const auto &scale = scaleVector[index];
			Scales::Id id{ (Scale::Type)type, Scales::Index{ index }};
			visitor(id, std::ref(scale));
		}
	}
}
