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

Marker::Marker(const Options &options,
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
		getValueForScale(scales, ScaleId::color, data, stats);

	auto lightness =
		getValueForScale(scales, ScaleId::lightness, data, stats);

	if (scales[ScaleId::color]->isPseudoDiscrete())
	{
		colorBuilder = ColorBuilder(style.plot.marker.lightnessRange(),
		    *style.plot.marker.colorPalette,
			(int)color,
		    lightness);
	}
	else
	{
		colorBuilder = ColorBuilder(style.plot.marker.lightnessRange(),
		    *style.plot.marker.colorGradient,
		    color,
		    lightness);
	}
	sizeFactor = getValueForScale(scales, ScaleId::size, data, stats,
								  options.subAxisOf(Scales::Id{ScaleId::size , scaleIndex}));
	sizeId = Id(data, scales[ScaleId::size]->discretesIds(), index);

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
	size.x = getValueForScale(scales, ScaleId::x, data, stats,
							  options.subAxisOf(Scales::Id{ScaleId::x , scaleIndex}),
							  !options.horizontal.get() && stackInhibitingShape);

	spacing.x = (options.horizontal.get()
				&& options.getScales().anyAxisSet()
				&& scales[ScaleId::x]->isPseudoDiscrete()) ? 1 : 0;

	position.y =
	size.y = getValueForScale(scales, ScaleId::y, data, stats,
							  options.subAxisOf(Scales::Id{ScaleId::y, scaleIndex}),
							  options.horizontal.get() && stackInhibitingShape);

	spacing.y = (!options.horizontal.get()
				&& options.getScales().anyAxisSet()
				&& scales[ScaleId::y]->isPseudoDiscrete()) ? 1 : 0;

	if (scales[ScaleId::label]->isEmpty())
		label = ::Anim::Weighted<Label>(Label(),0.0);
	else {
		auto value = getValueForScale(scales, ScaleId::label, data, stats);
		auto sliceIndex = data.subSliceIndex(scales[ScaleId::label]->discretesIds(), index);
		if (scales[ScaleId::label]->isPseudoDiscrete())
			label = Label(sliceIndex, data, table);
		else
			label = Label(value,
						  *scales[ScaleId::label]->continousId(),
						  sliceIndex, data, table);
	}
}

void Marker::setNextMarker(uint64_t itemId,
							  Marker *marker,
							  bool horizontal,
							  bool main)
{
	double Point::*coord = horizontal ? &Point::x : &Point::y;

	if (marker)
	{
		(main ? nextMainMarkerIdx : nextSubMarkerIdx) = marker->idx;

		if (main) marker->prevMainMarkerIdx = idx;

		if (itemId != 0)
		{
			marker->position.*coord += position.*coord;
		}
	}
}

void Marker::setIdOffset(size_t offset)
{
	if ((bool)prevMainMarkerIdx) (*prevMainMarkerIdx).value += offset;
	if ((bool)nextMainMarkerIdx) (*nextMainMarkerIdx).value += offset;
	if ((bool)nextSubMarkerIdx) (*nextSubMarkerIdx).value += offset;
}

std::string Marker::toJson(const Data::DataCube &data) const {
	auto cell = data.cellInfo(index);
	auto categories = Text::SmartString::map(cell.categories, [](const auto &pair) {
		auto key = Text::SmartString::escape(pair.first, "\"\\");
		auto value = Text::SmartString::escape(pair.second, "\"\\");
		return "\"" + key + "\":\"" + value + "\"";
	});
	auto values = Text::SmartString::map(cell.values, [](const auto &pair) {
		auto key = Text::SmartString::escape(pair.first, "\"\\");
		auto value = std::to_string(pair.second);
		return "\"" + key + "\":" + value;
	});
	return
		"\"marker\":{"
			"\"categories\":{"
				+ Text::SmartString::join(categories, ",") +
			"},"
			"\"values\":{"
				+ Text::SmartString::join(values, ",") +
			"},"
			"\"id\":"
				+ Text::SmartString::fromNumber(idx) +
		"}";
}

double Marker::getValueForScale(const Scales::Level &scales,
									 ScaleId type,
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
