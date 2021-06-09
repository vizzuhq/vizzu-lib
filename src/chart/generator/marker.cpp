#include "marker.h"

#include <iomanip>
#include <sstream>

#include "base/io/log.h"
#include "base/text/smartstring.h"
#include "chart/main/style.h"

#include "scalestats.h"

using namespace Vizzu;
using namespace Vizzu::Diag;
using namespace Geom;

Marker::Id::Id(const Data::DataCube &data,
					const Scale::DiscreteIndices &discretesIds,
					const Data::MultiDim::MultiIndex &index)
{
	seriesId = data.subSliceID(discretesIds, index);
	itemSliceIndex = data.subSliceIndex(discretesIds, index);
	itemId = data.getData().unfoldSubSliceIndex(itemSliceIndex);
}

Marker::Marker(const DiagramOptions &options,
						 const Styles::Chart &style,
						 const Data::DataCube &data,
						 const Data::DataTable &table,
						 ScalesStats &stats,
						 const Data::MultiDim::MultiIndex &index,
						 size_t idx)
	: index(index), idx(idx)
{
	enabled =  data.subCellSize() == 0
			|| !data.getData().at(index).subCells[0].isEmpty();

	auto scaleIndex = Scales::Index(data.repeatIndexAt(index));

	auto scales = options.getScales().getScales(scaleIndex);

	auto color =
		getValueForScale(scales, Scale::Color, data, stats);

	auto lightness =
		getValueForScale(scales, Scale::Lightness, data, stats);

	if (scales[Scale::Color]->isPseudoDiscrete())
	{
		colorBuilder = ColorBuilder(style.data.lightnessRange(),
		    *style.data.colorPalette,
			(int)color,
		    lightness);
	}
	else
	{
		colorBuilder = ColorBuilder(style.data.lightnessRange(),
		    *style.data.colorGradient,
		    color,
		    lightness);
	}
	sizeFactor = getValueForScale(scales, Scale::Size, data, stats,
								  options.subAxisOf(Scales::Id{Scale::Size , scaleIndex}));
	sizeId = Id(data, scales[Scale::Size]->discretesIds(), index);

	mainId = Id(data, options.mainAxis(scaleIndex).discretesIds(), index);

	bool stackInhibitingShape = options.shapeType.get() == ShapeType::Area;
	if (stackInhibitingShape) {
		Data::SeriesList subIds(options.subAxis(scaleIndex).discretesIds());
		subIds.remove(options.mainAxis(scaleIndex).discretesIds());
		subId = Id(data, subIds, index);
		Data::SeriesList stackIds(options.subAxis(scaleIndex).discretesIds());
		stackIds.section(options.mainAxis(scaleIndex).discretesIds());
		stackId = Id(data, stackIds, index);
	}
	else {
		stackId = subId = Id(data, options.subAxis(scaleIndex).discretesIds(), index);
	}

	position.x =
	size.x = getValueForScale(scales, Scale::X, data, stats,
							  options.subAxisOf(Scales::Id{Scale::X , scaleIndex}),
							  !options.horizontal.get() && stackInhibitingShape);

	spacing.x = (options.horizontal.get()
				&& options.getScales().anyAxisSet()
				&& scales[Scale::X]->isPseudoDiscrete()) ? 1 : 0;

	position.y =
	size.y = getValueForScale(scales, Scale::Y, data, stats,
							  options.subAxisOf(Scales::Id{Scale::Y, scaleIndex}),
							  options.horizontal.get() && stackInhibitingShape);

	spacing.y = (!options.horizontal.get()
				&& options.getScales().anyAxisSet()
				&& scales[Scale::Y]->isPseudoDiscrete()) ? 1 : 0;

	if (scales[Scale::Label]->isEmpty())
		label = ::Anim::Weighted<Label>(Label(),0);
	else {
		auto value = getValueForScale(scales, Scale::Label, data, stats);
		auto sliceIndex = data.subSliceIndex(scales[Scale::Label]->discretesIds(), index);
		if (scales[Scale::Label]->isPseudoDiscrete())
			label = Label(sliceIndex, data, table);
		else
			label = Label(value,
						  *scales[Scale::Label]->continousId(),
						  sliceIndex, data, table);
	}
}

void Marker::setNextItem(uint64_t itemId,
							  Marker *marker,
							  bool horizontal,
							  bool main)
{
	double Point::*coord = horizontal ? &Point::x : &Point::y;

	if (marker)
	{
		(main ? nextMainItemIdx : nextSubItemIdx) = marker->idx;

		if (main) marker->prevMainItemIdx = idx;

		if (itemId != 0)
		{
			marker->position.*coord += position.*coord;
		}
	}
}

void Marker::setIdOffset(size_t offset)
{
	if ((bool)prevMainItemIdx) (*prevMainItemIdx).value += offset;
	if ((bool)nextMainItemIdx) (*nextMainItemIdx).value += offset;
	if ((bool)nextSubItemIdx) (*nextSubItemIdx).value += offset;
}

double Marker::getValueForScale(const Scales::Level &scales,
									 Scale::Type type,
									 const Data::DataCube &data,
									 ScalesStats &stats,
									 const Scale *subScale,
									 bool inhibitStack) const
{
	const auto &scale = *scales[type];

	if (scale.isEmpty()) return scale.defaultValue();

	Scale::DiscreteIndices sumBy;

	if (subScale) {
		if (inhibitStack)
		{
			for (auto id : subScale->discretesIds())
				if (scale.isSeriesUsed(id))
					sumBy.pushBack(id);
		}
		else {
			sumBy = subScale->discretesIds();
			for (auto id : scale.discretesIds())
				sumBy.remove(id);
		}
	}

	auto continuous = scale.continousId();

	double value;
	double singlevalue;
	auto id = Id(data, scale.discretesIds(), index);

	auto &stat = stats.scales[type];

	if (continuous)
	{
		singlevalue = (double)data.valueAt(index, *continuous);

		if (scale.stackable())
			value = (double)data.aggregateAt(index, sumBy, *continuous);
		else
			value = singlevalue;
	}
	else
	{
		if (scale.stackable())
			value = 1.0;
		else
			value = (double)id.itemId;
	}

	if (enabled) {
		if (scale.isPseudoDiscrete()) stat.track(id);
		else {
			if (continuous) stat.trackSingle(singlevalue);
			stat.track(value);
		}
	}
	return value;
}

Circle Marker::toCircle() const
{
	return Circle(position, sqrt(sizeFactor));
}

void Marker::fromCircle(const Geom::Circle &circle)
{
	sizeFactor = circle.radius * circle.radius;
	position = circle.center;
}

Rect Marker::toRectangle() const
{
	return Rect(position - size, size);
}

void Marker::fromRectangle(const Rect &rect)
{
	position = rect.pos + rect.size;
	size = rect.size;
}

Math::Range<double> Marker::getSizeBy(bool horizontal) const
{
	return horizontal ? toRectangle().hSize() : toRectangle().vSize();
}

void Marker::setSizeBy(bool horizontal, const Math::Range<double> range)
{
	auto rect = toRectangle();
	if (horizontal) rect.setHSize(range);
	else rect.setVSize(range);
	fromRectangle(rect);
}

std::string Marker::getHint(const Data::DataCube &data, const Data::DataTable &table) const
{
	std::string hint;
	const auto &cell = data.getData().at(index);

	for (auto i = 0u; i < cell.subCells.size(); i++)
	{
		auto series = data.getSeriesBySubIndex(Data::SubCellIndex{i});

		if (series.getType() == Data::SeriesType::Exists) continue;

		auto value = (double)cell.subCells[i];

		if (!hint.empty()) hint += "\n";
		hint += Text::SmartString::fromNumber(value);
	}
	for (auto i = 0u; i < index.size(); i++)
	{
		if (!hint.empty()) hint += "\n";
		auto colIndex = data.getSeriesByDim(Data::MultiDim::DimIndex{i}).getColIndex();
		auto value = table.getInfo(colIndex).discreteValues()[index[i]];
		hint += value;
	}

	return hint;
}

Marker::Label::Label(const Data::MultiDim::SubSliceIndex &index,
						  const Data::DataCube &data,
						  const Data::DataTable &table)
	: hasValue(false), value(0.0)
{
	indexStr = getIndexString(index, data, table);
}

Marker::Label::Label(double value,
						  const Data::SeriesIndex &continous,
						  const Data::MultiDim::SubSliceIndex &index,
						  const Data::DataCube &data,
						  const Data::DataTable &table)
	: hasValue(true), value(value)
{
	unit = table.getInfo(continous.getColIndex()).getUnit();
	indexStr = getIndexString(index, data, table);
}

bool Marker::Label::operator==(const Marker::Label &other) const {
	return hasValue == other.hasValue
			&& value == other.value
			&& unit == other.unit
			&& indexStr == other.indexStr;
}

std::string Marker::Label::getIndexString(
	const Data::MultiDim::SubSliceIndex &index,
	const Data::DataCube &data,
	const Data::DataTable &table) const
{
	std::string res;

	for (auto i = 0u; i < index.size(); i++)
	{
		if (!res.empty()) res += ", ";
		auto colIndex = data.getSeriesByDim(index[i].dimIndex).getColIndex();
		auto value = table.getInfo(colIndex).discreteValues()[index[i].index];
		res += value;
	}
	return res;
}
