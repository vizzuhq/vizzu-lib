#include "marker.h"

#include "base/io/log.h"
#include "base/text/smartstring.h"
#include "chart/main/style.h"

#include "scalestats.h"

using namespace Vizzu;
using namespace Vizzu::Diag;
using namespace Geom;

Marker::Id::Id(const Data::DataCube &data,
    const Scale::DimensionIndices &dimensionIds,
    const Data::MultiDim::MultiIndex &index)
{
	seriesId = data.subSliceID(dimensionIds, index);
	itemSliceIndex = data.subSliceIndex(dimensionIds, index);
	itemId = data.getData().unfoldSubSliceIndex(itemSliceIndex);
}

Marker::Marker(const Options &options,
    const Styles::Chart &style,
    const Data::DataCube &data,
    const Data::DataTable &table,
    ScalesStats &stats,
    const Data::MultiDim::MultiIndex &index,
    size_t idx) :
    index(index),
    idx(idx)
{
	cellInfo = data.cellInfo(index);

	enabled = data.subCellSize() == 0
	       || !data.getData().at(index).subCells[0].isEmpty();

	const auto &scales = options.getScales();

	auto color =
	    getValueForScale(scales, ScaleId::color, data, stats);

	auto lightness =
	    getValueForScale(scales, ScaleId::lightness, data, stats);

	if (scales.at(ScaleId::color).isPseudoDimension()) {
		colorBuilder =
		    ColorBuilder(style.plot.marker.lightnessRange(),
		        *style.plot.marker.colorPalette,
		        (int)color,
		        lightness);
	}
	else {
		colorBuilder =
		    ColorBuilder(style.plot.marker.lightnessRange(),
		        *style.plot.marker.colorGradient,
		        color,
		        lightness);
	}
	sizeFactor = getValueForScale(scales,
	    ScaleId::size,
	    data,
	    stats,
	    options.subAxisOf(ScaleId::size));
	sizeId = Id(data, scales.at(ScaleId::size).dimensionIds(), index);

	mainId = Id(data, options.mainAxis().dimensionIds(), index);

	bool stackInhibitingShape =
	    options.shapeType.get() == ShapeType::Area;
	if (stackInhibitingShape) {
		Data::SeriesList subIds(options.subAxis().dimensionIds());
		subIds.remove(options.mainAxis().dimensionIds());
		subId = Id(data, subIds, index);
		Data::SeriesList stackIds(options.subAxis().dimensionIds());
		stackIds.section(options.mainAxis().dimensionIds());
		stackId = Id(data, stackIds, index);
	}
	else {
		stackId = subId =
		    Id(data, options.subAxis().dimensionIds(), index);
	}

	position.x = size.x = getValueForScale(scales,
	    ScaleId::x,
	    data,
	    stats,
	    options.subAxisOf(ScaleId::x),
	    !options.horizontal.get() && stackInhibitingShape);

	spacing.x =
	    (options.horizontal.get() && options.getScales().anyAxisSet()
	        && scales.at(ScaleId::x).isPseudoDimension())
	        ? 1
	        : 0;

	position.y = size.y = getValueForScale(scales,
	    ScaleId::y,
	    data,
	    stats,
	    options.subAxisOf(ScaleId::y),
	    options.horizontal.get() && stackInhibitingShape);

	spacing.y =
	    (!options.horizontal.get() && options.getScales().anyAxisSet()
	        && scales.at(ScaleId::y).isPseudoDimension())
	        ? 1
	        : 0;

	if (scales.at(ScaleId::label).isEmpty())
		label = ::Anim::Weighted<Label>(Label(), 0.0);
	else {
		auto value =
		    getValueForScale(scales, ScaleId::label, data, stats);
		auto sliceIndex = data.subSliceIndex(
		    scales.at(ScaleId::label).dimensionIds(),
		    index);
		if (scales.at(ScaleId::label).isPseudoDimension())
			label = Label(sliceIndex, data, table);
		else
			label = Label(value,
			    *scales.at(ScaleId::label).measureId(),
			    sliceIndex,
			    data,
			    table);
	}
}

void Marker::setNextMarker(uint64_t itemId,
    Marker *marker,
    bool horizontal,
    bool main)
{
	double Point::*coord = horizontal ? &Point::x : &Point::y;

	if (marker) {
		(main ? nextMainMarkerIdx : nextSubMarkerIdx) = marker->idx;

		if (main) marker->prevMainMarkerIdx = idx;

		if (itemId != 0) {
			marker->position.*coord += position.*coord;
		}
	}
}

void Marker::resetSize(bool horizontal)
{
	double Point::*coord = horizontal ? &Point::x : &Point::y;
	size.*coord = 0;
	position.*coord = 0;
}

void Marker::setIdOffset(size_t offset)
{
	if (prevMainMarkerIdx.hasOneValue())
		(*prevMainMarkerIdx).value += offset;
	if (nextMainMarkerIdx.hasOneValue())
		(*nextMainMarkerIdx).value += offset;
	if (nextSubMarkerIdx.hasOneValue())
		(*nextSubMarkerIdx).value += offset;
}

std::string Marker::toJson(const Data::DataTable &table) const
{
	auto categories = Text::SmartString::map(cellInfo.categories,
	    [&table](const auto &pair)
	    {
		    auto key =
		        Text::SmartString::escape(pair.first.toString(table),
		            "\"\\");
		    auto colIndex = pair.first.getColIndex();
		    auto numValue = table.getInfo(colIndex)
		                        .dimensionValues()[pair.second];
		    auto value = Text::SmartString::escape(numValue, "\"\\");
		    return "\"" + key + "\":\"" + value + "\"";
	    });
	auto values = Text::SmartString::map(cellInfo.values,
	    [&table](const auto &pair)
	    {
		    auto key =
		        Text::SmartString::escape(pair.first.toString(table),
		            "\"\\");
		    auto value = std::to_string(pair.second);
		    return "\"" + key + "\":" + value;
	    });
	return "{"
	       "\"categories\":{"
	     + Text::SmartString::join(categories, ",")
	     + "},"
	       "\"values\":{"
	     + Text::SmartString::join(values, ",")
	     + "},"
	       "\"id\":"
	     + std::to_string(idx) + "}";
}

double Marker::getValueForScale(const Scales &scales,
    ScaleId type,
    const Data::DataCube &data,
    ScalesStats &stats,
    const Scale *subScale,
    bool inhibitStack) const
{
	const auto &scale = scales.at(type);

	if (scale.isEmpty()) return scale.defaultValue();

	Scale::DimensionIndices sumBy;

	if (subScale) {
		if (inhibitStack) {
			for (auto id : subScale->dimensionIds())
				if (scale.isSeriesUsed(id)) sumBy.pushBack(id);
		}
		else {
			sumBy = subScale->dimensionIds();
			for (auto id : scale.dimensionIds()) sumBy.remove(id);
		}
	}

	auto measure = scale.measureId();

	double value;
	double singlevalue;
	auto id = Id(data, scale.dimensionIds(), index);

	auto &stat = stats.scales[type];

	if (scale.isPseudoDimension()) {
		if (scale.stackable())
			value = 1.0;
		else
			value = (double)id.itemId;
	}
	else {
		singlevalue = (double)data.valueAt(index, *measure);

		if (scale.stackable())
			value =
			    (double)data.aggregateAt(index, sumBy, *measure);
		else
			value = singlevalue;
	}

	if (enabled) {
		if (scale.isPseudoDimension())
			stat.track(id);
		else {
			if (measure) stat.trackSingle(singlevalue);
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

void Marker::setSizeBy(bool horizontal,
    const Math::Range<double> range)
{
	auto rect = toRectangle();
	if (horizontal)
		rect.setHSize(range);
	else
		rect.setVSize(range);
	fromRectangle(rect);
}

Marker::Label::Label(const Data::MultiDim::SubSliceIndex &index,
    const Data::DataCube &data,
    const Data::DataTable &table) :
    value(0.0),
    measureId(-1)
{
	indexStr = getIndexString(index, data, table);
}

Marker::Label::Label(double value,
    const Data::SeriesIndex &measure,
    const Data::MultiDim::SubSliceIndex &index,
    const Data::DataCube &data,
    const Data::DataTable &table) :
    value(value),
    measureId(measure.getColIndex())
{
	unit = table.getInfo(measureId).getUnit();
	indexStr = getIndexString(index, data, table);
}

bool Marker::Label::operator==(const Marker::Label &other) const
{
	return measureId == other.measureId && value == other.value
	    && unit == other.unit && indexStr == other.indexStr;
}

std::string Marker::Label::getIndexString(
    const Data::MultiDim::SubSliceIndex &index,
    const Data::DataCube &data,
    const Data::DataTable &table) const
{
	std::string res;

	for (auto i = 0u; i < index.size(); i++) {
		if (!res.empty()) res += ", ";
		auto colIndex =
		    data.getSeriesByDim(index[i].dimIndex).getColIndex();
		auto value =
		    table.getInfo(colIndex).dimensionValues()[index[i].index];
		res += value;
	}
	return res;
}
